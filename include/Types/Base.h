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
        VirtualType virtualType=VirtualType::NONVIRTUAL
    ):
    m_accessType(accessType),m_virtualType(virtualType)
    {}

    virtual ~Base(){}

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

    StaticType GetStaticType() const override
    {
        throw std::runtime_error("Base class Info has no static type");
    }

private:
    AccessType m_accessType;
    VirtualType m_virtualType;
};


#define BASE(CLASS, ACCESS, VIRT) new Base<CLASS>(AccessType::ACCESS, VirtualType::VIRT)

}