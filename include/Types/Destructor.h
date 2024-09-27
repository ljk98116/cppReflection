#pragma once

#include <memory>
#include <functional>
#include <type_traits>
#include <variant>

#include <include/Types/Object.h>
#include <include/Types/MemberInfo.h>
#include <include/Types/traits.h>

namespace Reflection
{

template <typename ClassT>
class DestructorInfo : public MemberInfo
{
public:
    DestructorInfo(AccessType access, VirtualType virtualType):
    m_name(Type2String<ClassT>() + "::destructor"),
    m_access(access),
    m_static(StaticType::NONE),
    m_virtual(virtualType),
    m_funcType(FuncType::Destructor)
    {}    

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
        return m_virtual;
    }

    std::string GetClassName() override
    {
        return Type2String<ClassT>();   
    } 

    Object Invoke(Object obj1) override
    {
        if(m_access == AccessType::PUBLIC)
        {
            ((ClassT)obj1).~ClassT();
            return Object(nullptr);
        }
        return Object(nullptr);
    }    
private:
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
};

#define DESTRUCTOR(ACCESS, VIRTUALFLAG, CLASS) \
    new DestructorInfo<CLASS>(AccessType::ACCESS, VirtualType::VIRTUALFLAG)
}