#include <include/Types/TypeFactory.h>

namespace Reflection
{
    TypeFactory& FactoryInstance()    
    {
        static TypeFactory factory;
        return factory;
    }
}