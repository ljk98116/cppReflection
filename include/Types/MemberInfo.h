#pragma once

#include <vector>
#include <string>
#include <memory>

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
    virtual VirtualType GetVirtualType() const = 0;
public:
    //property/field
    virtual void InvokeSet(Object& obj, const Object& value);
    virtual Object InvokeGet(Object& obj);
    //static property/field
    virtual void InvokeSet(const Object& value);
    virtual Object InvokeGet();

    virtual void DestroyObject(void *obj);
    virtual bool ReadOnly() const;

    virtual std::shared_ptr<MemberInfo> GetProperty(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> > GetProperties() const;
    virtual std::shared_ptr<MemberInfo> GetField(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> > GetFields() const;
    virtual std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> > GetBaseClasses() const;

    virtual void SetAccess(AccessType access);
    virtual void SetStaticType(StaticType staticType);
    virtual void SetVirtualType(VirtualType virtualType);
    virtual std::string GetClassName() = 0;
    virtual VirtualType GetInheritType();
    virtual size_t GetSize();
private:
};

}