#pragma once

#include <string>
#include <memory>
#include <vector>

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

enum FuncType
{
    Member,
    Constructor,
    Destructor,
    Normal
};

std::string AccessType2String(AccessType acc);
std::string StaticType2String(StaticType st);

std::shared_ptr<MemberInfo> GetMethod(const std::string& name, const std::vector<std::string>& args);
std::shared_ptr<MemberInfo> GetEnumType(const std::string& name);

int accu(int off, int sz);
int align(int off, int ref);
int getflag(int off);
}