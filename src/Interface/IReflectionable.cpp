#include <include/Interface/IReflectionable.h>
#include <include/Types/TypeFactory.h>

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

    std::shared_ptr<MemberInfo> GetMethod(const std::string& name, const std::vector<std::string>& args)
    {
        return FactoryInstance().GetMethod(name, args);
    }  
}