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

template <typename Ret, typename ...Args>
class NormalMethodInfo : public MemberInfo
{
public:
    using FuncT = std::function<Ret(Args...)>;
    using ArgsT = std::tuple<Args...>;
    using RetT = Ret;

    NormalMethodInfo(std::string name, Ret(*func)(Args...), AccessType access, StaticType staticType, VirtualType virt, FuncType funcType):
    m_name(name), m_func(func), m_access(access), m_static(staticType), m_virtual(virt), m_funcType(funcType)
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
        return "";
    }

    template <typename ...Rest>
    RetT invoke(Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {
            if(m_access == AccessType::PUBLIC) return m_func(std::forward<Args>(args)...);
            throw std::runtime_error("can not access");
        }
        else throw std::logic_error("not implemented");
    }

    Object Invoke(Object obj1) override
    {
        return invoke(std::forward<Object>(obj1));
    }

    Object Invoke(Object obj1, Object obj2) override
    {
        return invoke(std::forward<Object>(obj1), std::forward<Object>(obj2));
    }

    Object Invoke(Object obj1, Object obj2, Object obj3) override
    {
        return invoke(std::forward<Object>(obj1), std::forward<Object>(obj2), std::forward<Object>(obj3));
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4) override
    {
        return invoke(std::forward<Object>(obj1), std::forward<Object>(obj2), 
                    std::forward<Object>(obj3), std::forward<Object>(obj4));
    }
    //more params


private:
    FuncT m_func;
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
};

template <typename Ret, typename Class, typename ...Args>
class MemberMethodInfo : public MemberInfo
{
public:
    using MemberFuncT = Ret(Class::*)(Args...);
    using StaticMemberFuncT = Ret(*)(Args...);
    using ArgsT = std::tuple<Args...>;
    using RetT = Ret;
    using ClassT = Class;

    MemberMethodInfo(std::string name, Ret(Class::*func)(Args...), AccessType access, StaticType staticType, VirtualType virt, FuncType funcType):
    m_name(name), m_access(access), m_static(staticType), m_virtual(virt), m_funcType(funcType), m_func(func)
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

    //注意基类指针、引用调用子类虚方法时的情况，需要进行RTTI判断
    template <typename T, typename ...Rest>
    RetT invoke(T&& obj, Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {            
            return ((ClassT)(obj).*m_func)(std::forward<Rest>(args)...);
        }
        else throw std::logic_error("not implemented");
    }

    Object Invoke(Object obj1) override
    {
        return this->invoke<Object>(std::forward<Object>(obj1));
    }

    Object Invoke(Object obj1, Object obj2) override
    {
        return this->invoke<Object, Object>(std::forward<Object>(obj1), std::forward<Object>(obj2));
    }

    Object Invoke(Object obj1, Object obj2, Object obj3) override
    {
        return this->invoke<Object, Object, Object>(std::forward<Object>(obj1), std::forward<Object>(obj2), std::forward<Object>(obj3));
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4) override
    {
        return this->invoke<Object, Object, Object, Object>(std::forward<Object>(obj1), std::forward<Object>(obj2), 
                    std::forward<Object>(obj3), std::forward<Object>(obj4));
    }
    //more params

private:
    MemberFuncT m_func;
    StaticMemberFuncT m_staticFunc;
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
};

#define NORMALMETHOD(NAME, FUNC, STATICFLAG) \
    new NormalMethodInfo(#NAME, FUNC, AccessType::PUBLIC, StaticType::STATICFLAG, VirtualType::NONVIRTUAL, FuncType::Normal)

#define NORMALMEMBERMETHOD(NAME, FUNC, ACCESS, STATICFLAG, VIRTUALFLAG) \
    new MemberMethodInfo(#NAME, FUNC, AccessType::ACCESS, StaticType::STATICFLAG, VirtualType::VIRTUALFLAG, FuncType::Member)

#define CONSTRUCTORMETHOD(NAME, FUNC, ACCESS) \
    new MemberMethodInfo(#NAME, FUNC, AccessType::ACCESS, StaticType::NONE, VirtualType::NONVIRTUAL, FuncType::Constructor)

#define DESTRUCTORMETHOD(NAME, FUNC, ACCESS, VIRTUALFLAG) \
    new MemberMethodInfo(#NAME, FUNC, AccessType::ACCESS, StaticType::NONE, VirtualType::VIRTUALFLAG, FuncType::Destructor)

#define STATICMEMBERMETHOD(NAME, FUNC, ACCESS) \
    new NormalMethodInfo(#NAME, FUNC, AccessType::ACCESS, StaticType::STATIC, VirtualType::NONVIRTUAL, FuncType::Member)
}