#pragma once

#include <tuple>
#include <memory>
#include <functional>
#include "MemberInfo.h"
#include "Object.h"
#include <variant>
#include <iostream>

namespace Reflection
{

template <typename ClassT_, typename T>
class PropertyInfo : public MemberInfo
{
    using Set_Method = std::function<void(Object&, const T&)>;
    using Get_Method = std::function<T(Object&)>;
    using Static_Set_Method = std::function<void(const T&)>;
    using Static_Get_Method = std::function<T()>;
public:
    using ClassT = ClassT_;
    //类成员属性
    PropertyInfo(
        std::string name,
        T (ClassT::*memberPtr),
        std::variant<Set_Method, std::nullptr_t> set_func,
        std::variant<Get_Method, std::nullptr_t> get_func,
        bool readonly = false,
        AccessType accessType=AccessType::PRIVATE,
        StaticType staticType=StaticType::NONE
    ):
    m_propName(name), m_accessType(accessType), 
    m_staticType(staticType), m_memberPtr(memberPtr),m_readonly(readonly)
    {
        if(std::holds_alternative<Set_Method>(set_func)) m_setFunc = std::get<Set_Method>(set_func);
        if(std::holds_alternative<Get_Method>(get_func)) m_getFunc = std::get<Get_Method>(get_func);   
    }
    //静态成员属性
    PropertyInfo(
        std::string name,
        T *staticMemPtr,
        std::variant<Static_Set_Method, std::nullptr_t> set_func,
        std::variant<Static_Get_Method, std::nullptr_t> get_func,
        bool readonly = false,
        AccessType accessType=AccessType::PRIVATE,
        StaticType staticType=StaticType::STATIC
    ):
    m_propName(name), m_accessType(accessType), 
    m_staticType(StaticType::STATIC), m_staticMemPtr(staticMemPtr), m_readonly(readonly)
    {
        if(std::holds_alternative<Static_Set_Method>(set_func)) m_staticSetFunc = std::get<Static_Set_Method>(set_func);
        if(std::holds_alternative<Static_Get_Method>(get_func)) m_staticGetFunc = std::get<Static_Get_Method>(get_func);   
    }

    virtual ~PropertyInfo(){}
    std::string GetClassName() override
    {
        return Type2String<ClassT>();
    }
    
    std::string Name() const override
    {
        return m_propName;
    }

    AccessType GetAccess() const override
    {
        return m_accessType;
    }

    StaticType GetStaticType() const override
    {
        return m_staticType;
    }

    VirtualType GetVirtualType() const override
    {
        throw std::runtime_error("propertyinfo is nonvirtual");
    }

    void SetAccess(AccessType access) override
    {
        m_accessType = access;
    }

    void SetStaticType(StaticType staticType) override
    {
        m_staticType = staticType;
    }

    void SetVirtualType(VirtualType virtualType) override
    {
        throw std::runtime_error("propertyInfo is nonvirtual");
    }

    bool ReadOnly() const override
    {
        return m_readonly;
    }
    
    size_t GetSize() override
    {
        return sizeof(ClassT);
    }
    //注意子类object访问基类保护属性的情况,需要看obj对应的类是否为ClassT的子类
    //memberinfo需要能获取子类的类型
    void InvokeSet(Object& obj, const Object& value) override
    {
        if(m_readonly) throw std::runtime_error("try to modify readonly attr");
        if(!Accessable(obj)) throw std::runtime_error("access propertyInfo denied");

        //获取obj的typeInfo,看是否为本属性所在类的基类,根据虚表数量计算指针的偏移位置，需要改变obj
        auto obj_typeInfo = obj.GetTypeInfo();
        auto base_Infos = obj_typeInfo->GetBaseClasses();
        size_t off = 0;
        if(obj_typeInfo->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
        for(auto base_info : base_Infos)
        {
            if(base_info->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
            if(base_info->GetClassName() == Type2String<ClassT>()) break;
            off += base_info->GetSize();
        }
        auto val = value.GetData<T>();
        if(m_staticType == StaticType::STATIC) m_staticSetFunc(val);
        else
        {
            if(off == 0) m_setFunc(obj, val);
            else
            {
                //提取基类对象
                void *basePtr = (void*)((uintptr_t)obj.Data().get() + off);
                Object obj2(*(ClassT*)basePtr);
                m_setFunc(obj2, val);
                //内存操作
                memcpy((void*)((uintptr_t)obj.Data().get() + off), obj2.Data().get(), sizeof(ClassT));
            }
        }
    }
    //静态属性，看是否能访问到
    void InvokeSet(const Object& value) override
    {
        if(m_readonly) throw std::runtime_error("try to modify readonly static attr");
        if(m_accessType != AccessType::PUBLIC) throw std::runtime_error("try to modify private/protect static attr");

        auto val = value.GetData<T>();

        if(m_staticType == StaticType::STATIC) m_staticSetFunc(val);
        else throw std::runtime_error("try to modify nonstatic member without object ptr");
    }

    Object InvokeGet(Object& obj) override
    {
        if(!Accessable(obj)) throw std::runtime_error("access propertyInfo denied");
        if(m_staticType == StaticType::STATIC) return m_staticGetFunc();
        return m_getFunc(obj);
    }

    Object InvokeGet() override
    {
        if(m_accessType != AccessType::PUBLIC) throw std::runtime_error("access nonpublic attr");
        if(m_staticType != StaticType::STATIC) throw std::runtime_error("try to access nonstatic member without object ptr");
        return m_staticGetFunc();
    }
    template <typename Class, typename T_>
    bool operator==(const PropertyInfo<Class, T_>& rhs)
    {
        return Type2String<ClassT>() == Type2String<Class>() && Name() == rhs.Name();
    }
private:
    bool Accessable(const Object& obj)
    {
        auto typeInfo = obj.GetTypeInfo();
        return m_accessType == AccessType::PUBLIC;
    }
    const Set_Method default_setfunc = [this](Object& obj, const T& value)
    {
        ClassT tmp = obj.GetData<ClassT>();
        tmp.*m_memberPtr = value;
        obj.SetData(tmp);
    };

    const Get_Method default_getfunc = [this](Object& obj)->T
    {
        ClassT tmp = obj.GetData<ClassT>();
        return tmp.*m_memberPtr;
    };

    const Static_Set_Method default_static_setfunc = [this](const T& value)
    {
        *m_staticMemPtr = value;
    };

    const Static_Get_Method default_static_getfunc = [this]()
    {
        return *m_staticMemPtr;
    };
    Set_Method m_setFunc = default_setfunc;
    Get_Method m_getFunc = default_getfunc;

    Static_Set_Method m_staticSetFunc = default_static_setfunc;
    Static_Get_Method m_staticGetFunc = default_static_getfunc;
    std::string m_propName = "Unknown";
public:
    AccessType m_accessType;
    StaticType m_staticType;
private:
    T (ClassT::*m_memberPtr);
    T *m_staticMemPtr;
    bool m_readonly;
};

#define SETFUNC(CLASS, NAME, MEMBER) \
    [](Object& obj, const decltype(CLASS::MEMBER)& value) \
    { \
        CLASS obj_ = obj.GetData<CLASS>(); \
        obj_.set##NAME(value); \
        obj.SetData(obj_);\
    }

#define GETFUNC(CLASS, NAME) \
    [](Object& obj) \
    { \
        CLASS obj_ = obj.GetData<CLASS>(); \
        return obj_.get##NAME(); \
    }

#define STATIC_SETFUNC(CLASS, NAME, MEMBER) \
    [](const decltype(CLASS::MEMBER)& value) \
    { \
        CLASS::set##NAME(value); \
    }

#define STATIC_GETFUNC(CLASS, NAME) \
    []() \
    { \
        return CLASS::get##NAME(); \
    }

#define PROPERTY(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, SETFUNC(CLASS, NAME, MEMBER), GETFUNC(CLASS, NAME), false, AccessType::ACCESS, StaticType::STATICFLAG)

#define PROPERTYREADONLY(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, GETFUNC(CLASS, NAME), true, AccessType::ACCESS, StaticType::STATICFLAG)    

#define PROPERTYDEFAULT(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, nullptr, false, AccessType::ACCESS, StaticType::STATICFLAG)

#define PROPERTYREADONLYDEFAULT(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, nullptr, true, AccessType::ACCESS, StaticType::STATICFLAG)

#define STATIC_PROPERTY(CLASS, MEMBER, NAME, ACCESS) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, STATIC_SETFUNC(CLASS, NAME, MEMBER), STATIC_GETFUNC(CLASS, NAME), false, AccessType::ACCESS, StaticType::STATIC)

#define STATIC_PROPERTYREADONLY(CLASS, MEMBER, NAME, ACCESS) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, STATIC_GETFUNC(CLASS, NAME), true, AccessType::ACCESS, StaticType::STATIC)    

#define STATIC_PROPERTYDEFAULT(CLASS, MEMBER, NAME, ACCESS) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, nullptr, false, AccessType::ACCESS, StaticType::STATIC)

#define STATIC_PROPERTYREADONLYDEFAULT(CLASS, MEMBER, NAME, ACCESS) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, nullptr, true, AccessType::ACCESS, StaticType::STATIC)

#define SET(NAME, TYPE) \
    void set##NAME(const TYPE& value)

#define GET(NAME, TYPE) \
    TYPE get##NAME()

#define STATIC_SET(NAME, TYPE) \
    static void set##NAME(const TYPE& value)

#define STATIC_GET(NAME, TYPE) \
    static TYPE get##NAME()
}
