#pragma once

#include <string>
namespace Reflection
{

enum AccessType
{
    PUBLIC,
    PRIVATE,
    PROTECT
};

enum StaticType
{
    STATIC,
    NONE
};

std::string AccessType2String(AccessType acc);
std::string StaticType2String(StaticType st);

}