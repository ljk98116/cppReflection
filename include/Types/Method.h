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
    {
        if constexpr (sizeof...(Args) > 0) init_args<Args...>();
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
        return m_virtual;
    }

    std::string GetClassName() override
    {
        return "";
    }

    template <typename ...Rest>
    Object invoke(Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {
            if(m_access == AccessType::PUBLIC)
            {
                if constexpr (std::is_same<RetT, void>::value)
                {
                    m_func(std::forward<Args>(Args(args))...);
                    return Object(nullptr);
                }
                else return m_func(std::forward<Args>(Args(args))...);
            }
            throw std::runtime_error("can not access");
        }
        else throw std::logic_error("not implemented");
        if constexpr (std::is_same<RetT, void>::value) return Object(nullptr);
        else return Object(nullptr);
    }

    Object Invoke(Object obj1) override
    {
        return invoke(obj1);
    }

    Object Invoke(Object obj1, Object obj2) override
    {
        return invoke(obj1, obj2);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3) override
    {
        return invoke(obj1, obj2, obj3);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4) override
    {
        return invoke(obj1, obj2, obj3, obj4);
    }
    //more params
    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5, Object obj6) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5, obj6);
    }
    
    std::vector<std::string>& GetArg() override
    {
        return m_args;
    }
private:
    template <typename T, typename ...Rest>
    void init_args()
    {
        m_args.push_back(Type2String<T>());
        if constexpr (sizeof...(Rest)) init_args<Rest...>();
    }
    FuncT m_func;
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
    std::vector<std::string> m_args;
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
    m_name(init_method_name(name)), m_access(access), m_static(staticType), m_virtual(virt), m_funcType(funcType), m_func(func)
    {
        if constexpr (sizeof...(Args) > 0) init_args<Args...>();
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
        return m_virtual;
    }

    std::string GetClassName() override
    {
        return Type2String<ClassT>();   
    }

    std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name) override
    {
        auto typeInfo = typeof(ClassT);
        return typeInfo.GetBaseClass(name);
    }

    std::vector<std::string>& GetArg() override
    {
        return m_args;
    }

    //注意基类指针、引用调用子类虚方法时的情况，需要进行RTTI判断
    template <typename T, typename ...Rest>
    Object invoke(T&& obj, Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {            
            if constexpr (std::is_same<RetT, void>::value)
            {
                if(obj.IsPointer() && *(ClassT**)(obj.GetRawData()) != nullptr) ((*(ClassT**)(obj.GetRawData()))->*m_func)(std::forward<Args>(Args(args))...);
                else if(obj.IsRef()) (((std::reference_wrapper<ClassT>)(obj)).get().*m_func)(std::forward<Args>(Args(args))...);
                else
                {
                    if constexpr (std::is_abstract_v<ClassT>) throw std::runtime_error("abstract cast");
                    else ((ClassT)(obj).*m_func)(std::forward<Args>(Args(args))...);
                }
                return Object(nullptr);
            }
            else
            {
                if(obj.IsPointer() && *(ClassT**)(obj.GetRawData()) != nullptr) return ((*(ClassT**)(obj.GetRawData()))->*m_func)(std::forward<Args>(Args(args))...);
                else if(obj.IsRef()) return (((std::reference_wrapper<ClassT>)(obj)).get().*m_func)(std::forward<Args>(Args(args))...);
                else
                {
                    if constexpr (std::is_abstract_v<ClassT>) throw std::runtime_error("abstract cast");
                    else return ((ClassT)(obj).*m_func)(std::forward<Args>(Args(args))...);                    
                }              
            }
        }
        else throw std::logic_error("not implemented");
        return Object(nullptr);
    }

    Object Invoke(Object obj1) override
    {
        return invoke(obj1);
    }

    Object Invoke(Object obj1, Object obj2) override
    {
        return invoke(obj1, obj2);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3) override
    {
        return invoke(obj1, obj2, obj3);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4) override
    {
        return invoke(obj1, obj2, obj3, obj4);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5, Object obj6) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5, obj6);
    }
    //more params

private:
    template <typename T, typename ...Rest>
    std::string ParseArgsImpl()
    {
        std::string ret = Type2String<T>();
        if constexpr (sizeof...(Rest) == 0) return ret;
        else
        {
            ret += ",";
            return ret + ParseArgsImpl<Rest...>();
        }
    }
    template <typename ...T>
    std::string ParseArgs()
    {
        if constexpr (sizeof...(T) == 0) return "";
        else return ParseArgsImpl<T...>();
    }

    std::string init_method_name(const std::string& name)
    {
        if constexpr (sizeof...(Args) == 0) return name;
        else
        {
            return name + "(" + ParseArgs<Args...>() + ")";
        }
    }

    template <typename T, typename ...Rest>
    void init_args()
    {
        m_args.push_back(Type2String<T>());
        if constexpr (sizeof...(Rest)) init_args<Rest...>();
    }

    MemberFuncT m_func;
    StaticMemberFuncT m_staticFunc;
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
    std::vector<std::string> m_args;
};

#define ARGS(...) __VA_ARGS__

#define NORMALMETHOD(RET, NAME, ARGS, STATICFLAG) \
    new NormalMethodInfo(#NAME, (RET(*)(ARGS))&NAME, AccessType::PUBLIC, StaticType::STATICFLAG, VirtualType::NONVIRTUAL, FuncType::Normal)    

#define NORMALMEMBERMETHOD(RET, CLASS, NAME, ARGS, ACCESS, STATICFLAG, VIRTUALFLAG) \
    new MemberMethodInfo(#NAME, (RET(CLASS::*)(ARGS))&CLASS::NAME, AccessType::ACCESS, StaticType::STATICFLAG, VirtualType::VIRTUALFLAG, FuncType::Member)

#define STATICMEMBERMETHOD(RET, CLASS, NAME, ARGS, ACCESS) \
    new NormalMethodInfo(#NAME, (RET(*)(ARGS))&CLASS::NAME, AccessType::ACCESS, StaticType::STATIC, VirtualType::NONVIRTUAL, FuncType::Member)
}