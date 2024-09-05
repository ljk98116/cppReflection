#pragma once

#include <include/Interface/IReflectionable.h>
namespace Reflection
{
    std::string AccessType2String(AccessType acc)
    {
        if(acc == AccessType::PRIVATE) return "PRIVATE";
        else if(acc == AccessType::PROTECT) return "PROTECT";
        else if(acc == AccessType::PUBLIC) return "PUBLIC";
        return "";
    }

    std::string StaticType2String(StaticType st)
    {
        if(st == StaticType::NONE) return "NONE";
        else if(st == StaticType::STATIC) return "STATIC";
        return "";
    }
}