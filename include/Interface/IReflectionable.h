#pragma once

#include <string>
#include <memory>

namespace Reflection
{

class MemberInfo;
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

enum VirtualType
{
    VIRTUAL,
    NONVIRTUAL
};

std::string AccessType2String(AccessType acc);
std::string StaticType2String(StaticType st);

}