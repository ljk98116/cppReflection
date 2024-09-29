#pragma once

#include <type_traits>
#include <utility>
#include <tuple>

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


//截取长度N后的部分
template <int idx, int N, typename ...T>
struct SequenceSuffix_Impl;

template <int idx, int N, typename T, typename ...Rest>
struct SequenceSuffix_Impl<idx, N, T, Rest...>
{
    using type = std::conditional_t<
        std::is_same_v<std::integral_constant<int, idx>, std::integral_constant<int, N> >,
        std::tuple<T, Rest...>,
        typename SequenceSuffix_Impl<idx + 1, N, Rest...>::type
    >;
};

template <int idx, int N>
struct SequenceSuffix_Impl<idx, N>
{
    using type =std::tuple<>;
};

template <int N, typename ...T>
using SequenceSuffix = typename SequenceSuffix_Impl<0, N, T...>::type;

template<typename T>
struct tuple_concat
{
    template <typename ...Rest>
    std::tuple<T, Rest...> concat(std::tuple<Rest...> arg){}
};

//截取长度N后的部分
template <int N, typename ...T>
struct SequencePrefix_Impl;

template <int N, typename T, typename ...Rest>
struct SequencePrefix_Impl<N, T, Rest...>
{
    using type = std::conditional_t<
        std::is_same_v<std::integral_constant<int, N>, std::integral_constant<int, 0>>,
        std::tuple<>,
        decltype(tuple_concat<T>().concat(typename SequencePrefix_Impl<N-1, Rest...>::type{}))
    >;
};

template <int N>
struct SequencePrefix_Impl<N>
{
    using type = std::tuple<>;
};

template <int N, typename ...T>
using SequencePrefix = typename SequencePrefix_Impl<N, T...>::type;

template <typename ...T>
struct CastTypes
{
    template <typename ...U>
    auto operator()(U&&... args)
    {
        return std::make_tuple(T(std::forward<U>(args))...);
    }
};

}