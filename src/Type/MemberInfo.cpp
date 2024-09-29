#include <include/Types/MemberInfo.h>
#include <include/Types/Object.h>

namespace Reflection
{

static auto ret = std::vector<std::shared_ptr<MemberInfo> >();
static auto ret2 = std::vector<std::string>();

void MemberInfo::InvokeSet(Object& obj, const Object& value)
{

}

Object MemberInfo::InvokeGet(Object& obj)
{
    return Object(nullptr);
}

void MemberInfo::InvokeSet(const Object& value)
{

}

Object MemberInfo::InvokeGet()
{
    return Object(nullptr);
}

void MemberInfo::DestroyObject(void *obj)
{
    
}

bool MemberInfo::ReadOnly() const
{
    return false;
}

std::shared_ptr<MemberInfo> MemberInfo::GetProperty(const std::string& name)
{
    return nullptr;
}

std::vector<std::shared_ptr<MemberInfo> >& MemberInfo::GetProperties()
{
    throw std::logic_error("not implemented");
    return ret;
}

std::shared_ptr<MemberInfo> MemberInfo::GetField(const std::string& name)
{
    return nullptr;
}

std::vector<std::shared_ptr<MemberInfo> >& MemberInfo::GetFields()
{
    throw std::logic_error("not implemented");
    return ret;
}

std::shared_ptr<MemberInfo> MemberInfo::GetBaseClass(const std::string& name)
{
    return nullptr;
}
std::vector<std::shared_ptr<MemberInfo>>& MemberInfo::GetBaseClasses()
{
    throw std::logic_error("not implemented");
    return ret;
}

std::shared_ptr<MemberInfo> MemberInfo::GetMethod(const std::string& name)
{
    return nullptr;
}

std::vector<std::shared_ptr<MemberInfo>>& MemberInfo::GetMethods()
{
    throw std::logic_error("not implemented");
    return ret;
}

std::shared_ptr<MemberInfo> MemberInfo::GetConstructor(const std::vector<std::string>& arg_vec)
{
    throw std::logic_error("not implemented");
    return nullptr;
}

std::vector<std::shared_ptr<MemberInfo>>& MemberInfo::GetConstructors()
{
    throw std::logic_error("not implemented");
    return ret;
}

std::shared_ptr<MemberInfo> MemberInfo::GetDestructor()
{
    throw std::logic_error("not implemented");
    return nullptr;    
}

std::shared_ptr<MemberInfo> MemberInfo::GetEnum(const std::string& name)
{
    throw std::logic_error("not implemented");
    return nullptr;
}

std::vector<std::shared_ptr<MemberInfo>>& MemberInfo::GetEnums()
{
    throw std::logic_error("not implemented");
    return ret;
}

std::vector<std::string>& MemberInfo::GetArg()
{
    throw std::runtime_error("not implemented");
    return ret2;
}

Object MemberInfo::GetEnumValue(const std::string& name)
{
    throw std::runtime_error("not implemented");
    return Object(nullptr);
}

size_t MemberInfo::GetSize()
{
    return 0;
}

VirtualType MemberInfo::GetInheritType()
{
    return VirtualType::NONVIRTUAL;
}

Object MemberInfo::Invoke()
{
    throw std::logic_error("not implemented");
}

Object MemberInfo::Invoke(Object obj1)
{
    throw std::logic_error("not implemented");
}

Object MemberInfo::Invoke(Object obj1, Object obj2)
{
    throw std::logic_error("not implemented");
}

Object MemberInfo::Invoke(Object obj1, Object obj2, Object obj3)
{
    throw std::logic_error("not implemented");
}

Object MemberInfo::Invoke(Object obj1, Object obj2, Object obj3, Object obj4)
{
    throw std::logic_error("not implemented");
}

Object MemberInfo::Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5)
{
    throw std::logic_error("not implemented");
}

Object MemberInfo::Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5, Object obj6)
{
    throw std::logic_error("not implemented");
}
void MemberInfo::SetAccess(AccessType access){}
void MemberInfo::SetStaticType(StaticType staticType){}
void MemberInfo::SetVirtualType(VirtualType virtualType){}

}