#pragma once

#include <type_traits>
#include <utility>

namespace Reflection
{

template<typename T>
struct HasRegister
{
private:
    template<typename U>
    static auto Check(int) -> decltype(std::declval<U>().Register(), std::true_type());
    template<typename U>
    static std::false_type Check(...);
public:
    enum { value = std::is_same<decltype(Check<T>(0)), std::true_type>::value  };
};

struct Unknown{};

namespace detail
{
    template <typename T>
    struct MemberFunctionImpl
    {
        using ClassType = Unknown;
        using RetType = Unknown;
    };

    template <typename Ret, typename ClassT, typename ...Args>
    struct MemberFunctionImpl<Ret(ClassT::*)(Args...)>
    {
        using ClassType = ClassT;
        using RetType = Ret;
    };

    template <typename T>
    struct NormalFunctionImpl
    {
        using RetType = Unknown;
    };

    template <typename Ret, typename ...Args>
    struct NormalFunctionImpl<Ret(*)(Args...)>
    {
        using RetType = Ret;
    };    
}

template <typename T> 
struct FunctionClass
{
    using RawType = std::remove_cv_t<std::remove_reference_t<T> >;
    using type = std::conditional_t<
        std::is_member_function_pointer_v<RawType>,
        typename detail::MemberFunctionImpl<RawType>::ClassType,
        Unknown
    >;
};

template <typename T> 
struct FunctionRet
{
    using RawType = std::remove_cv_t<std::remove_reference_t<T> >;
    using type = std::conditional_t<
        std::is_member_function_pointer_v<RawType>,
        typename detail::MemberFunctionImpl<RawType>::ClassType,
        typename detail::NormalFunctionImpl<RawType>::RetType
    >;
};

template <typename T>
using GetFunctionClass = typename FunctionClass<T>::type;

template <typename T>
using GetFunctionRet = typename FunctionRet<T>::type;

}