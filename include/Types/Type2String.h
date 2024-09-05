#pragma once
#include <string>

namespace Reflection
{

template <typename T>
std::string Type2String()
{
    std::string func_name = __PRETTY_FUNCTION__;
    size_t idx1 = func_name.find_first_of('[');
    size_t idx2 = func_name.find_first_of('=', idx1 + 1);
    size_t idx3 = func_name.find_first_of(';', idx2 + 1);
    return func_name.substr(idx2 + 2, idx3 - (idx2 + 2));
}

}