#pragma once

#include <vector>
#include <string>
#include <memory>
#include <variant>

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
    virtual std::vector<std::shared_ptr<MemberInfo> >& GetProperties();
    virtual std::shared_ptr<MemberInfo> GetField(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> >& GetFields();
    virtual std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> >& GetBaseClasses();
    virtual std::shared_ptr<MemberInfo> GetMethod(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> >& GetMethods();
    virtual std::shared_ptr<MemberInfo> GetEnum(const std::string& name);
    virtual std::vector<std::shared_ptr<MemberInfo> >& GetEnums();
    virtual size_t GetAlignment();

    virtual std::shared_ptr<MemberInfo> GetConstructor(const std::vector<std::string>& arg_vec);
    virtual std::vector<std::shared_ptr<MemberInfo> >& GetConstructors();

    virtual std::shared_ptr<MemberInfo> GetDestructor();
    virtual std::vector<std::string>& GetArg();

    virtual Object GetEnumValue(const std::string& name);
    virtual void SetAccess(AccessType access);
    virtual void SetStaticType(StaticType staticType);
    virtual void SetVirtualType(VirtualType virtualType);
    virtual std::string GetClassName() = 0;
    virtual VirtualType GetInheritType();
    virtual size_t GetSize();

    //父类接口，子类实现，参数传入子类的变长参数invoke,左值或者右值作为参数
    virtual Object Invoke();
    virtual Object Invoke(Object obj1);
    virtual Object Invoke(Object obj1, Object obj2);
    virtual Object Invoke(Object obj1, Object obj2, Object obj3);
    virtual Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4);
    virtual Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5);
    virtual Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5, Object obj6);
private:
};


}