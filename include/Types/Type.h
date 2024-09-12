#pragma once

#include "Type2String.h"

#include "PropertyList.h"
#include "FieldList.h"
#include "BaseList.h"

#include "traits.h"

#include <vector>
#include <type_traits>
#include <stdexcept>

namespace Reflection
{
//包含ClassT_的所有属性信息，包括父类
template <typename ClassT_>
class Type : public MemberInfo
{
public:
    using ClassT = ClassT_;
    Type(){}
    virtual ~Type(){}
    static auto create()
    {
        return Type<ClassT_>();
    }
    //explicit Type(PropertyList propList, FieldList_ fieldList, FunctionList_ funcList):
    //m_propList(propList), m_fieldList(fieldList), m_funcList(funcList)
    //{}

    std::string Name() const override
    {
        return m_name;
    }
    AccessType GetAccess() const override
    {
        return AccessType::PUBLIC;
    }

    StaticType GetStaticType() const override
    {
        return StaticType::NONE;
    }

    VirtualType GetVirtualType() const override
    {
        return VirtualType::NONVIRTUAL;
    }

    template <typename Property>
    auto AddProperty(Property* prop)
    {
        m_propList.AddProperty(prop);
        return *this;
    }   

    std::shared_ptr<MemberInfo> GetProperty(const std::string& name) override
    {
        return m_propList.GetProperty(name);
    }

    std::vector<std::shared_ptr<MemberInfo> > GetProperties() const override
    {
        return m_propList.GetProperties();
    }

    template <typename Field>
    auto AddField(Field* prop)
    {
        m_fieldList.AddField(prop);
        return *this;
    }   

    std::shared_ptr<MemberInfo> GetField(const std::string& name) override
    {
        return m_fieldList.GetField(name);
    }

    std::vector<std::shared_ptr<MemberInfo> > GetFields() const override
    {
        return m_fieldList.GetFields();
    }

    //设置父类
    template <typename Base>
    auto AddBaseClass(Base *base)
    {
        //判断继承关系
        static_assert(std::is_base_of<typename Base::ClassT, ClassT>::value);
        //注册基类，将基类的属性信息加入本类的对应集合
        if constexpr (!HasRegister<typename Base::ClassT>::value) throw std::logic_error("can not reflect class");
        else
        {
            //可能出现递归调用，但只有一层
            auto typeInfo = Type<typename Base::ClassT>().Register();
            std::shared_ptr<decltype(typeInfo)> childPtr = std::make_shared<decltype(typeInfo)>();
            *childPtr = typeInfo;
            RegisterBase(std::static_pointer_cast<MemberInfo>(childPtr));
            m_baseList.AddBaseClass(base);
        }
        return *this;
    }

    std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name) override
    {
        return m_baseList.GetBaseClass(name);
    }    

    std::vector<std::shared_ptr<MemberInfo> > GetBaseClasses() const override
    {
        return m_baseList.GetBaseClasses();
    }    

    auto Register()
    {
        if constexpr (HasRegister<ClassT>::value) return ClassT::Register();
        else return Type<ClassT>();
    }

    void DestroyObject(void *obj) override
    {
        ClassT *data = reinterpret_cast<ClassT*>(obj);
        delete data;
    }
private:
    //需要更改属性是否可见
    void RegisterBase(std::shared_ptr<MemberInfo> baseClass)
    {
        auto access = baseClass->GetAccess();
        auto virt = baseClass->GetVirtualType();
        //更改属性是否可见
        auto props = baseClass->GetProperties();
        auto fields = baseClass->GetFields();
        for(auto& prop : props)
        {
            if(access == AccessType::PROTECT && prop->GetAccess() != AccessType::PRIVATE) prop->SetAccess(AccessType::PROTECT);
            else if(access == AccessType::PRIVATE) prop->SetAccess(AccessType::PRIVATE);
        }
        for(auto& field : fields)
        {
            if(access == AccessType::PROTECT && field->GetAccess() != AccessType::PRIVATE) field->SetAccess(AccessType::PROTECT);
            else if(access == AccessType::PRIVATE) field->SetAccess(AccessType::PRIVATE);
        }
        m_propList = m_propList + baseClass->GetProperties();
        m_fieldList = m_fieldList + baseClass->GetFields();
        m_baseList = m_baseList + baseClass->GetBaseClasses();
    }
    PropertyList m_propList;
    FieldList m_fieldList;
    //基类信息
    BaseList m_baseList;
    //FunctionList m_funcList;
    static std::string m_name;
};

template <typename ClassT_>
std::string Type<ClassT_>::m_name = Type2String<ClassT_>();

#define typeof(CLASS) Type<CLASS>().Register()

}