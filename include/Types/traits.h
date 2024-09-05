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

}