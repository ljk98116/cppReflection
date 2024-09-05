#pragma once

#include "Type2String.h"
#include "PropertyList.h"
#include "FieldList.h"
#include "traits.h"

#include <vector>
#include <type_traits>
#include <stdexcept>

namespace Reflection
{
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

    template <typename Property>
    auto AddProperty(Property* prop)
    {
        m_propList.AddProperty(prop);
        return *this;
    }   

    std::shared_ptr<MemberInfo> GetProperty(const std::string& name)
    {
        return m_propList.GetProperty(name);
    }

    std::vector<std::shared_ptr<MemberInfo> > GetProperties() const
    {
        return m_propList.GetProperties();
    }

    template <typename Field>
    auto AddField(Field* prop)
    {
        m_fieldList.AddField(prop);
        return *this;
    }   

    std::shared_ptr<MemberInfo> GetField(const std::string& name)
    {
        return m_fieldList.GetField(name);
    }

    std::vector<std::shared_ptr<MemberInfo> > GetFields() const
    {
        return m_fieldList.GetFields();
    }

    auto Register()
    {
        if constexpr (HasRegister<ClassT>::value) return ClassT::Register();
        else return Type<ClassT>();
    }

    void DestroyObject(void *obj) override
    {
        ClassT *data = (ClassT*)obj;
        delete data;
    }
private:
    PropertyList m_propList;
    FieldList m_fieldList;
    //FunctionList m_funcList;
    static std::string m_name;
};

template <typename ClassT_>
std::string Type<ClassT_>::m_name = Type2String<ClassT_>();

#define typeof(CLASS) Type<CLASS>().Register()

}