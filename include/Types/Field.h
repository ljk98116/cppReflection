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
    explicit FieldInfo(
        std::string name,
        MemberT_ memberPtr,
        AccessType accessType=AccessType::PRIVATE,
        StaticType staticType=StaticType::NONE
    ):
    m_propName(name), m_accessType(accessType), 
    m_staticType(staticType), m_memberPtr(memberPtr)
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

    StaticType GetVirtualType() const override
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
private:
    std::string m_propName = "Unknown";
    AccessType m_accessType;
    StaticType m_staticType;
    MemberT_ m_memberPtr;
};   

#define FIELD(CLASS, MEMBER, ACCESS, STATICFLAG) \
    new FieldInfo<CLASS, decltype(CLASS::MEMBER), decltype(&CLASS::MEMBER)>(#MEMBER, &CLASS::MEMBER, AccessType::ACCESS, StaticType::STATICFLAG)
}