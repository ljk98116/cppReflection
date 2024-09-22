#include <include/Types/MemberInfo.h>
#include <include/Types/Object.h>

namespace Reflection
{
static auto ret = std::vector<std::shared_ptr<MemberInfo> >();
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

std::shared_ptr<MemberInfo> MemberInfo::GetConstructor(const std::string& name)
{
    return nullptr;
}

std::vector<std::shared_ptr<MemberInfo>>& MemberInfo::GetConstructors()
{
    throw std::logic_error("not implemented");
    return ret;
}

size_t MemberInfo::GetSize()
{
    return 0;
}

VirtualType MemberInfo::GetInheritType()
{
    return VirtualType::NONVIRTUAL;
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

void MemberInfo::SetAccess(AccessType access){}
void MemberInfo::SetStaticType(StaticType staticType){}
void MemberInfo::SetVirtualType(VirtualType virtualType){}

}