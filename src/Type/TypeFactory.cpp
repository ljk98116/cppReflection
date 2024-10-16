#include <include/Types/TypeFactory.h>

namespace Reflection
{
    void(*Regfunc)() = nullptr;

    static TypeFactory factory;

    TypeFactory& FactoryInstance()    
    {
        static bool init_flag = false;
        if(!init_flag && Regfunc != nullptr)
        {
            init_flag = true;
            Regfunc();
        } 
        return factory;
    }

    TypeRegister::TypeRegister(void(*func)())
    {
        Regfunc = func;
    }
}