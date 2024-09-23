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

template <typename ClassT, typename ...Args>
class ConstructorInfo : public MemberInfo
{
public:
    explicit ConstructorInfo(AccessType access=AccessType::PUBLIC):
    m_name(Type2String<ClassT>() + "::constructor"),
    m_access(access),
    m_static(StaticType::NONE),
    m_virtual(VirtualType::NONVIRTUAL),
    m_funcType(FuncType::Constructor)
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

    //注意基类指针、引用调用子类虚方法时的情况，需要进行RTTI判断
    template <typename ...Rest>
    Object invoke(Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {            
            //调用对应的构造函数
            if(m_access == AccessType::PUBLIC)
            {
                ClassT ret = ClassT(std::forward<Args>(args)...);
                return Object(ret);
            }
        }
        else
        {
            throw std::logic_error("not implemented");
            return Object(nullptr);
        }
        return Object(nullptr);
    }

    Object Invoke() override
    {
        return invoke();
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

    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
    std::vector<std::string> m_args;
};

#define CONSTRUCTOR(ACCESS, ...) \
    new ConstructorInfo<__VA_ARGS__>(AccessType::ACCESS)

template <typename T, typename ...Rest>
static void InitArgsImpl(std::vector<std::string>& ret)
{
    ret.push_back(Type2String<T>());
    if constexpr (sizeof...(Rest) > 0) InitArgsImpl<Rest...>();
}

template <typename ...T>
static std::vector<std::string> InitArgs()
{
    if constexpr (sizeof...(T) == 0) return std::vector<std::string>();
    else
    {
        auto ret = std::vector<std::string>();
        InitArgsImpl<T...>(ret);
        return ret;
    }
}

#define ARGTYPE(...) \
    InitArgs<__VA_ARGS__>()

}