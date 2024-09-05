#pragma once

#include <vector>
#include <string>
#include <include/Interface/IReflectionable.h>

namespace Reflection
{
class Object;
class MemberInfo
{
public:
    virtual std::string Name() const = 0;
    virtual AccessType GetAccess() const = 0;
    virtual StaticType GetStaticType() const = 0;
//特定类型具有的虚方法
public:
    virtual void InvokeSet(Object& obj, const Object& value);
    virtual Object InvokeGet(Object& obj);
    virtual void DestroyObject(void *obj);
private:

};

}