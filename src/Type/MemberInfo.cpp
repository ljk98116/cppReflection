#include <include/Types/MemberInfo.h>
#include <include/Types/Object.h>

namespace Reflection
{

void MemberInfo::InvokeSet(Object& obj, const Object& value)
{

}

Object MemberInfo::InvokeGet(Object& obj)
{
    return Object(nullptr);
}

void MemberInfo::DestroyObject(void *obj)
{
    
}
}