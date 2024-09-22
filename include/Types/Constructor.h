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

template <typename Arg, typename ...Rest>
std::string init_constructor_name(std::string&& orin)
{
    if constexpr (sizeof...(Rest) == 0) return orin + ">";
    else 
    {
        orin += ",";
        auto tmp = orin + Type2String<Arg>();
        return init_constructor_name<Rest...>(std::move(tmp));
    }
}

template <typename ClassT, typename ...Args>
class ConstructorInfo : public MemberInfo
{
public:
    explicit ConstructorInfo(AccessType access=AccessType::PUBLIC):
    m_name(init_constructor_name<Args...>(Type2String<ClassT>() + "<")),
    m_access(access),
    m_static(StaticType::NONE),
    m_virtual(VirtualType::NONVIRTUAL),
    m_funcType(FuncType::Constructor)
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
                return Object(ClassT(std::forward<Rest>(args)...));
            }
        }
        else 
        {
            throw std::logic_error("not implemented");
            return Object(nullptr);
        }
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


private:
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
};

#define CONSTRUCTOR(ACCESS, ...) \
    new ConstructorInfo<__VA_ARGS__>(AccessType::ACCESS)

}