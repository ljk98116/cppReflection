#include <include/Types/Object.h>

namespace Reflection
{

Object::~Object()
{
    m_typeInfo->DestroyObject(m_data);
}

}