#pragma once

#include <tuple>
#include <memory>
#include <functional>
#include "MemberInfo.h"
#include "Object.h"
#include <variant>

namespace Reflection
{

template <typename ClassT_, typename T>
class PropertyInfo : public MemberInfo
{
    using Set_Method = std::function<void(Object&, const T&)>;
    using Get_Method = std::function<T(Object&)>;
public:
    using ClassT = ClassT_;
    explicit PropertyInfo(
        std::string name,
        T (ClassT::*memberPtr),
        std::variant<Set_Method, std::nullptr_t> set_func,
        std::variant<Get_Method, std::nullptr_t> get_func,
        bool readonly = false,
        AccessType accessType=AccessType::PRIVATE,
        StaticType staticType=StaticType::NONE
    ):
    m_propName(name), m_accessType(accessType), 
    m_staticType(staticType), m_memberPtr(memberPtr),m_readonly(readonly)
    {
        if(std::holds_alternative<Set_Method>(set_func)) m_setFunc = std::get<Set_Method>(set_func);
        if(std::holds_alternative<Get_Method>(get_func)) m_getFunc = std::get<Get_Method>(get_func);   
    }

    std::string Name() const override
    {
        return m_propName;
    }
    AccessType GetAccess() const override
    {
        return m_accessType;
    }

    StaticType GetStaticType() const override
    {
        return m_staticType;
    }

    bool ReadOnly() const override
    {
        return m_readonly;
    }

    void InvokeSet(Object& obj, const Object& value) override
    {
        if(m_readonly) throw std::runtime_error("try to modify readonly attr");
        auto val = value.GetData<T>();
        m_setFunc(obj, val);
    }

    Object InvokeGet(Object& obj) override
    {
        return m_getFunc(obj);
    }

private:
    const Set_Method default_setfunc = [this](Object& obj, const T& value)
    {
        ClassT tmp = obj.GetData<ClassT>();
        tmp.*m_memberPtr = value;
        obj.SetData(tmp);
    };

    const Get_Method default_getfunc = [this](Object& obj)->T
    {
        ClassT tmp = obj.GetData<ClassT>();
        return tmp.*m_memberPtr;
    };

    Set_Method m_setFunc = default_setfunc;
    Get_Method m_getFunc = default_getfunc;

    std::string m_propName = "Unknown";
    AccessType m_accessType;
    StaticType m_staticType;
    T (ClassT::*m_memberPtr);
    bool m_readonly;
};

#define SETFUNC(CLASS, NAME, MEMBER) \
    [](Object& obj, const decltype(CLASS::MEMBER)& value) \
    { \
        CLASS obj_ = obj.GetData<CLASS>(); \
        obj_.set##NAME(value); \
        obj.SetData(obj_);\
    }

#define GETFUNC(CLASS, NAME) \
    [](Object& obj) \
    { \
        CLASS obj_ = obj.GetData<CLASS>(); \
        return obj_.get##NAME(); \
    }

#define PROPERTY(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, SETFUNC(CLASS, NAME, MEMBER), GETFUNC(CLASS, NAME), false, AccessType::ACCESS, StaticType::STATICFLAG)

#define PROPERTYREADONLY(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, GETFUNC(CLASS, NAME), true, AccessType::ACCESS, StaticType::STATICFLAG)    

#define PROPERTYDEFAULT(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, nullptr, false, AccessType::ACCESS, StaticType::STATICFLAG)

#define PROPERTYREADONLYDEFAULT(CLASS, MEMBER, NAME, ACCESS, STATICFLAG) \
    new PropertyInfo<CLASS, decltype(CLASS::MEMBER)> \
    (#NAME, &CLASS::MEMBER, nullptr, nullptr, true, AccessType::ACCESS, StaticType::STATICFLAG)

#define SET(NAME, TYPE) \
    void set##NAME(const TYPE& value)

#define GET(NAME, TYPE) \
    TYPE get##NAME()
}



