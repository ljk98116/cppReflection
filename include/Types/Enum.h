#pragma once

#include <memory>
#include <functional>
#include <type_traits>
#include <variant>
#include <unordered_map>
#include <cstdarg>
#include <utility>

#include <include/Types/Object.h>
#include <include/Types/MemberInfo.h>
#include <include/Types/traits.h>

namespace Reflection
{

//需要利用哈希表来保存
//成员枚举类型和普通枚举类型

//类中的枚举类型，需要实现InvokeGet和InvokeSet
template <typename ClassT_, typename Enum_>
class MemberEnumInfo : public MemberInfo
{
public:
    using ClassT = ClassT_;
    using Enum = Enum_;
    //参数包为字符串->枚举值对
    MemberEnumInfo(std::string name, Enum ClassT::* val, AccessType access, StaticType staticType, std::vector<std::pair<std::string, int> > init_params):
    m_val(val),m_access(access), m_static(staticType), 
    m_name(name)
    {
        for(int i=0;i<init_params.size();++i)
        {
            auto item = init_params[i];
            m_enumVals[item.first] = item.second;
        }
    }

    MemberEnumInfo(std::string name, Enum* val, AccessType access, StaticType staticType, std::vector<std::pair<std::string, int> > init_params):
    m_staticVal(val),m_access(access), m_static(staticType),
    m_name(Type2String<ClassT>() + "::" + name)
    {
        for(int i=0;i<init_params.size();++i)
        {
            auto item = init_params[i];
            m_enumVals[item.first] = item.second;
        }
    }
    std::string Name() const override
    {
        return m_name;
    }

    AccessType GetAccess() const override
    {
        return m_access;
    }

    StaticType GetStaticType() const override
    {
        return m_static;
    }

    VirtualType GetVirtualType() const override
    {
        return VirtualType::NONVIRTUAL;
    }

    std::string GetClassName() override
    {
        return Type2String<ClassT>();
    }    

    Object InvokeGet(Object& obj) override
    {
        if(!Accessable(obj)) throw std::runtime_error("access enuminfo denied");
        //获取obj的typeInfo,看是否为本属性所在类的基类,根据虚表数量计算指针的偏移位置，需要改变obj
        auto obj_typeInfo = obj.GetTypeInfo();
        auto base_Infos = obj_typeInfo->GetBaseClasses();
        size_t off = 0;
        //当前class具有虚表指针？
        //if(obj_typeInfo->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
        //看基类是否有虚表指针
        for(auto base_info : base_Infos)
        {
            if(base_info->GetClassName() == Type2String<ClassT>()) break;
            off += base_info->GetSize();
        }
        bool baseprop = Type2String<ClassT>() != obj_typeInfo->GetClassName();
        Object obj2((ClassT)(*(ClassT*)((uintptr_t)obj.Data().get() + (baseprop ? off : 0))));
        return ((ClassT)obj2).*m_val;
    }

    void InvokeSet(Object& obj, const Object& value) override
    {
        if(!Accessable(obj)) throw std::runtime_error("access enumInfo denied");

        //获取obj的typeInfo,看是否为本属性所在类的基类,根据虚表数量计算指针的偏移位置，需要改变obj
        auto obj_typeInfo = obj.GetTypeInfo();
        auto base_Infos = obj_typeInfo->GetBaseClasses();
        size_t off = 0;
        //当前class具有虚表指针？
        //if(obj_typeInfo->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
        for(auto base_info : base_Infos)
        {
            if(base_info->GetClassName() == Type2String<ClassT>()) break;
            off += base_info->GetSize();
        }
        bool baseprop = Type2String<ClassT>() != obj_typeInfo->GetClassName();
        auto val = value.GetData<int>();
        if(off == 0 || !baseprop)
        {
            ClassT& objData = *(ClassT*)(obj.Data().get());
            objData.*m_val = (Enum)val;         
        }
        else
        {
            //提取基类对象
            void *basePtr = (void*)((uintptr_t)obj.Data().get() + off);
            auto value_ = *(ClassT*)basePtr;
            Object obj2(value_);
            ClassT& objData = *(ClassT*)(obj2.Data().get());
            objData.*m_val = (Enum)val;
            //内存操作
            memcpy((void*)((uintptr_t)obj.Data().get() + off), obj2.Data().get(), sizeof(ClassT));
        }
    }

    void InvokeSet(const Object& value) override
    {
        if(m_access != AccessType::PUBLIC) throw std::runtime_error("try to modify private/protect static enum");

        auto val = value.GetData<Enum>();

        if(m_static == StaticType::STATIC) *m_staticVal = (Enum)val;
        else throw std::runtime_error("try to modify nonstatic member without object ptr");
    }

    Object InvokeGet() override
    {
        if(m_access != AccessType::PUBLIC) throw std::runtime_error("access nonpublic enum");
        if(m_static != StaticType::STATIC) throw std::runtime_error("try to access nonstatic member without object ptr");
        return (Enum)*m_staticVal;
    }

    Object GetEnumValue(const std::string& name) override
    {
        return m_enumVals[name];
    }

private:
    bool Accessable(const Object& obj)
    {
        auto typeInfo = obj.GetTypeInfo();
        return m_access == AccessType::PUBLIC;
    }
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    std::unordered_map<std::string, int> m_enumVals;
    Enum (ClassT::* m_val);
    Enum *m_staticVal;
};

template <typename Enum_>
class NormalEnumInfo : public MemberInfo
{
public:
    using Enum = Enum_;
    //参数包为字符串->枚举值对
    NormalEnumInfo(StaticType staticType, std::vector<std::pair<std::string, int> > init_params):
    m_static(staticType), 
    m_name(Type2String<Enum>())
    {
        for(int i=0;i<init_params.size();++i)
        {
            auto item = init_params[i];
            m_enumVals[item.first] = item.second;
        }
    }

    std::string Name() const override
    {
        return m_name;
    }

    AccessType GetAccess() const override
    {
        return AccessType::PUBLIC;
    }

    StaticType GetStaticType() const override
    {
        return m_static;
    }

    VirtualType GetVirtualType() const override
    {
        return VirtualType::NONVIRTUAL;
    }

    std::string GetClassName() override
    {
        throw std::runtime_error("Normal Enum has no class");
        return "";
    }    

    Object GetEnumValue(const std::string& name) override
    {
        return m_enumVals[name];
    }
private:
    std::string m_name;
    StaticType m_static;
    std::unordered_map<std::string, int> m_enumVals;
};

#define ENUMVALUE(ENUM,VALUE) std::pair<std::string, int>{#VALUE, ENUM::VALUE}
#define STATICMEMBERENUM(NAME, CLASS, MEMBER, ACCESS, ...) \
    new MemberEnumInfo(#NAME, &CLASS::MEMBER, StaticType::STATIC, std::vector<std::pair<std::string, int> >{__VA_ARGS__})

#define NORMALMEMBERENUM(NAME, CLASS, MEMBER, ACCESS, ...) \
    new MemberEnumInfo(#NAME, &CLASS::MEMBER, AccessType::ACCESS, StaticType::NONE, std::vector<std::pair<std::string, int> >{__VA_ARGS__})

#define NORMALENUM(ENUM, STATICFLAG, ...) \
    new NormalEnumInfo<ENUM>(StaticType::STATICFLAG, std::vector<std::pair<std::string, int> >{__VA_ARGS__})

}