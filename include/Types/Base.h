#pragma once

#include <tuple>
#include <memory>
#include <functional>
#include "MemberInfo.h"
#include "Object.h"
#include <variant>

namespace Reflection
{

template <typename ClassT_>
class Base : public MemberInfo
{
public:
    using ClassT = ClassT_;
    //需要利用TypeInfo进行初始化
    Base(
        AccessType accessType=AccessType::PRIVATE, 
        VirtualType virtualType=VirtualType::NONVIRTUAL,
        VirtualType inheritVirtType=VirtualType::NONVIRTUAL
    ):
    m_accessType(accessType),m_virtualType(virtualType), m_inherit(inheritVirtType)
    {}

    virtual ~Base(){}
    std::string GetClassName() override
    {
        return Type2String<ClassT>();
    }

    std::string Name() const override
    {
        return Type2String<ClassT>();
    }

    AccessType GetAccess() const override
    {
        return m_accessType;
    }

    VirtualType GetVirtualType() const override
    {
        return m_virtualType;
    }

    size_t GetSize() override
    {
        return m_sz;
    }
    StaticType GetStaticType() const override
    {
        throw std::runtime_error("Base class Info has no static type");
    }
    VirtualType GetInheritType() override
    {
        return m_inherit;
    }
    void SetSize(size_t sz)
    {
        m_sz = sz;
    }
    template <typename T>
    bool operator==(const Base<T>& rhs)
    {
        return Type2String<ClassT>() == Type2String<T>();
    }
private:
    AccessType m_accessType;
    VirtualType m_virtualType;
    VirtualType m_inherit;
    size_t m_sz = sizeof(ClassT);
};


#define BASE(CLASS, ACCESS, VIRT, Inherit) new Base<CLASS>(AccessType::ACCESS, VirtualType::VIRT, VirtualType::Inherit)

}