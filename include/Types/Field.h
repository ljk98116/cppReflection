#pragma once

#include <tuple>
#include <memory>
#include "Object.h"
#include "MemberInfo.h"

namespace Reflection
{
template <typename ClassT_, typename T, typename MemberT_>
class FieldInfo : public MemberInfo
{
public:
    using ClassT = ClassT_;
    using MemberT = MemberT_;
    FieldInfo(
        std::string name,
        MemberT_ memberPtr,
        AccessType accessType=AccessType::PRIVATE,
        StaticType staticType=StaticType::NONE
    ):
    m_propName(name), m_accessType(accessType), 
    m_staticType(staticType), m_memberPtr(memberPtr), m_staticMemPtr(nullptr)
    {}

    FieldInfo(
        std::string name,
        T* staticMemberPtr,
        AccessType accessType=AccessType::PRIVATE
    ):
    m_propName(name), m_accessType(accessType), 
    m_staticType(StaticType::STATIC), m_memberPtr(nullptr),
    m_staticMemPtr(staticMemberPtr)
    {}

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
        throw std::runtime_error("fieldinfo is nonvirtual");
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
        throw std::runtime_error("fieldInfo is nonvirtual");
    }

    size_t GetSize() override
    {
        return sizeof(ClassT);
    }

    void InvokeSet(Object& obj, const Object& value) override
    {
        if(!Accessable(obj)) throw std::runtime_error("access fieldInfo denied");

        //获取obj的typeInfo,看是否为本属性所在类的基类,根据虚表数量计算指针的偏移位置，需要改变obj
        auto obj_typeInfo = obj.GetTypeInfo();
        auto base_Infos = obj_typeInfo->GetBaseClasses();
        size_t off = 0;
        //当前class具有虚表指针？
        if(obj_typeInfo->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
        //看基类是否有虚表指针
        for(auto base_info : base_Infos)
        {
            if(base_info->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
            if(base_info->GetClassName() == Type2String<ClassT>()) break;
            off += base_info->GetSize();
        }
        auto val = value.GetData<T>();
        if(m_staticType == StaticType::STATIC) *m_staticMemPtr = val;
        else
        {
            auto basePtr = (ClassT*)((uintptr_t)obj.Data().get() + off);
            *basePtr.*m_memberPtr = val;
        }
    }

    //静态属性，看是否能访问到
    void InvokeSet(const Object& value) override
    {
        if(m_accessType != AccessType::PUBLIC) throw std::runtime_error("try to modify private/protect static field");

        auto val = value.GetData<T>();

        if(m_staticType == StaticType::STATIC) *m_staticMemPtr = val;
        else throw std::runtime_error("try to modify nonstatic member without object ptr");
    }    

    Object InvokeGet(Object& obj) override
    {
        if(!Accessable(obj)) throw std::runtime_error("access propertyInfo denied");
        if(m_staticType == StaticType::STATIC) return *m_staticMemPtr;
        //获取obj的typeInfo,看是否为本属性所在类的基类,根据虚表数量计算指针的偏移位置，需要改变obj
        auto obj_typeInfo = obj.GetTypeInfo();
        auto base_Infos = obj_typeInfo->GetBaseClasses();
        size_t off = 0;
        //当前class具有虚表指针？
        if(obj_typeInfo->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
        //看基类是否有虚表指针
        for(auto base_info : base_Infos)
        {
            if(base_info->GetVirtualType() == VirtualType::VIRTUAL) off += 8;
            if(base_info->GetClassName() == Type2String<ClassT>()) break;
            off += base_info->GetSize();
        }
        if(m_staticType == StaticType::STATIC) return *m_staticMemPtr;
        auto basePtr = (ClassT*)((uintptr_t)obj.Data().get() + off);
        return *basePtr.*m_memberPtr;
    }

    Object InvokeGet() override
    {
        if(m_accessType != AccessType::PUBLIC) throw std::runtime_error("access nonpublic attr");
        if(m_staticType != StaticType::STATIC) throw std::runtime_error("try to access nonstatic member without object ptr");
        return *m_staticMemPtr;
    }    
private:
    bool Accessable(const Object& obj)
    {
        auto typeInfo = obj.GetTypeInfo();
        return m_accessType == AccessType::PUBLIC;
    }
    std::string m_propName = "Unknown";
    AccessType m_accessType;
    StaticType m_staticType;
    MemberT_ m_memberPtr;
    T *m_staticMemPtr;
};   

#define FIELD(CLASS, MEMBER, ACCESS, STATICFLAG) \
    new FieldInfo<CLASS, decltype(CLASS::MEMBER), decltype(&CLASS::MEMBER)>(#MEMBER, &CLASS::MEMBER, AccessType::ACCESS, StaticType::STATICFLAG)
}