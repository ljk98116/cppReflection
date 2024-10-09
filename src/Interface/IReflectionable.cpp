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

    std::shared_ptr<MemberInfo> GetEnumType(const std::string& name)
    {
        return FactoryInstance().GetEnumType(name);
    }

    int getflag(int off)
    {
        int f1 = 0;
        if(off == 0) f1 = 0;
        else if(off % 16 == 0) f1 = 16;
        else if(off % 8 == 0) f1 = 8;
        else if(off % 4 == 0) f1 = 4;
        else if(off % 2 == 0) f1 = 2;
        else f1 = 1;
        return f1;
    }

    int accu(int off, int sz)
    {
        int f1 = getflag(off);
        int f2 = getflag(sz);
        if(f1 < f2) off = (off / f2 + (off % f2 > 0)) * f2;
        else sz = (sz / f1 + (sz % f1 > 0)) * f1;
        return off + sz;
    }

    int align(int off, int ref)
    {
        int f1 = getflag(off);
        int f2 = getflag(ref);
        int maxval = std::max(f1, f2);
        off = (off / maxval + (off % maxval > 0)) * maxval;
        return off;
    }
}