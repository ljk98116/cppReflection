#pragma once

#include "Type2String.h"

#include "PropertyList.h"
#include "FieldList.h"
#include "BaseList.h"
#include "MethodList.h"
#include "ConstructorList.h"

#include "TypeFactory.h"
#include "traits.h"

#include <vector>
#include <type_traits>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace Reflection
{

template <typename ClassT_>
class Type;

template <typename ClassT>
class DestructorInfo;

template <typename T>
static std::shared_ptr<MemberInfo> AsMemberInfo(Type<T>&& item);


struct compare_MemberInfoPtr
{
    bool operator()(std::shared_ptr<MemberInfo> item1, std::shared_ptr<MemberInfo> item2)
    {
        return item1->Name() == item2->Name();
    }
};

//包含ClassT_的所有属性信息，包括父类
template <typename ClassT_>
class Type : public MemberInfo
{
public:
    using ClassT = ClassT_;

    explicit Type(VirtualType virt = VirtualType::NONVIRTUAL):
    m_virt(virt)
    {}

    virtual ~Type(){}

    static auto create()
    {
        return Type<ClassT_>();
    }
    //explicit Type(PropertyList propList, FieldList_ fieldList, FunctionList_ funcList):
    //m_propList(propList), m_fieldList(fieldList), m_funcList(funcList)
    //{}
    std::string GetClassName() override
    {
        return Type2String<ClassT>();
    }
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
        return m_virt;
    }

    template <typename Property>
    auto AddProperty(Property* prop)
    {
        //没有出现过，或者出现过且与当类类名相同时替换
        if(m_propList.GetProperty(prop->Name()) == nullptr) m_propList.AddProperty(prop);
        else if(prop->GetClassName() == Type2String<ClassT>() || prop->GetBaseClass(Type2String<ClassT>()) != nullptr)
        {
            std::shared_ptr<MemberInfo> ptr = std::shared_ptr<MemberInfo>(prop);
            for(auto& prop_ : m_propList.GetProperties())
            {
                if(prop_->Name() == prop->Name()) prop_ = ptr;
            }
        }
        return *this;
    }   

    size_t GetSize() override
    {
        return sizeof(ClassT);
    }
    
    std::shared_ptr<MemberInfo> GetProperty(const std::string& name) override
    {
        return m_propList.GetProperty(name);
    }

    std::vector<std::shared_ptr<MemberInfo> >& GetProperties() override
    {
        return m_propList.GetProperties();
    }

    template <typename Field>
    auto AddField(Field* prop)
    {
        //没有出现过，或者出现过且与当类类名相同时替换
        if(m_fieldList.GetField(prop->Name()) == nullptr) m_fieldList.AddField(prop);
        else if(prop->GetClassName() == Type2String<ClassT>() || prop->GetBaseClass(Type2String<ClassT>()) != nullptr)
        {
            std::shared_ptr<MemberInfo> ptr = std::shared_ptr<MemberInfo>(prop);
            for(auto& field_ : m_fieldList.GetFields())
            {
                if(field_->Name() == prop->Name()) field_ = ptr;
            }
        }
        return *this;
    }   

    std::shared_ptr<MemberInfo> GetField(const std::string& name) override
    {
        return m_fieldList.GetField(name);
    }

    std::vector<std::shared_ptr<MemberInfo> >& GetFields() override
    {
        return m_fieldList.GetFields();
    }

    template <typename Method>
    auto AddMethod(Method* method)
    {
        //没有出现过，或者出现过且与当类类名相同时替换
        if(m_funcList.GetMethod(method->Name()) == nullptr) m_funcList.AddMethod(method);
        else if(method->GetClassName() == Type2String<ClassT>() || method->GetBaseClass(Type2String<ClassT>()) != nullptr)
        {
            std::shared_ptr<MemberInfo> ptr = std::shared_ptr<MemberInfo>(method);
            for(auto& method_ : m_funcList.GetMethods())
            {
                if(method_->Name() == method->Name()) method_ = ptr;
            }
        }
        return *this;
    }

    template <typename Constructor>
    auto AddConstructor(Constructor *constructor)
    {
        m_constructorList.AddMethod(constructor);
        return *this;
    }

    std::shared_ptr<MemberInfo> GetMethod(const std::string& name) override
    {
        return m_funcList.GetMethod(name);
    }

    std::vector<std::shared_ptr<MemberInfo> >& GetMethods()override
    {
        return m_funcList.GetMethods();
    }    

    std::shared_ptr<MemberInfo> GetConstructor(const std::vector<std::string>& args) override
    {
        return m_constructorList.GetConstructor(args);
    }

    std::vector<std::shared_ptr<MemberInfo> >& GetConstructors() override
    {
        return m_constructorList.GetConstructors();
    }

    auto AddDestructor(DestructorInfo<ClassT> *destructor) 
    {
        m_destructor = std::shared_ptr<MemberInfo>(destructor);
        return *this;
    }

    std::shared_ptr<MemberInfo> GetDestructor() override
    {
        return m_destructor;
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
            RegisterBase(std::static_pointer_cast<MemberInfo>(childPtr), base);
            m_baseList.AddBaseClass(base);
        }
        return *this;
    }

    std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name) override
    {
        return m_baseList.GetBaseClass(name);
    }    

    std::vector<std::shared_ptr<MemberInfo> >& GetBaseClasses() override
    {
        return m_baseList.GetBaseClasses();
    }    

    auto Register()
    {
        if constexpr (HasRegister<ClassT>::value) 
        {
            auto ret = ClassT::Register();
            if(!FactoryInstance().HasKey(typeid(ClassT).name()))
                FactoryInstance().insert(std::make_pair(std::string(typeid(ClassT).name()), AsMemberInfo(std::move(ret))));
            return ret;
        }
        else return Type<ClassT>();
    }

    void DestroyObject(void *obj) override
    {
        ClassT *data = reinterpret_cast<ClassT*>(obj);
        delete data;
    }

private:
    //需要更改属性是否可见
    template <typename Base>
    void RegisterBase(std::shared_ptr<MemberInfo> baseClass, Base *base)
    {
        auto access = base->GetAccess();
        auto virt = base->GetVirtualType();
        //更改属性是否可见
        auto props = baseClass->GetProperties();
        auto fields = baseClass->GetFields();
        auto methods = baseClass->GetMethods();
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
        for(auto& method : methods)
        {
            if(access == AccessType::PROTECT && method->GetAccess() != AccessType::PRIVATE) method->SetAccess(AccessType::PROTECT);
            else if(access == AccessType::PRIVATE) method->SetAccess(AccessType::PRIVATE);
        }
        //需要处理菱形继承问题，父类信息如果重复出现，看父类的baseList是否带有虚属性，如果没有抛异常
        //内存对齐,减去补齐到对应倍数的父类,虚继承和前面的父类成员对齐，没有父类默认向8的倍数对齐，虚表指针
        for(int i=0;i<baseClass->GetBaseClasses().size();++i)
        {
            auto item = baseClass->GetBaseClasses()[i];
            size_t sz = item->GetSize();
            if(i == 0 && item->GetInheritType() == VirtualType::VIRTUAL)
            {
                //虚函数类唯一继承虚基类，与虚表指针对齐
                if(item->GetVirtualType() == VirtualType::VIRTUAL)
                {
                    sz = (item->GetSize() / 8 + ((item->GetSize() % 8) != 0)) * 8;
                }
                else
                {
                    //gcc默认与4对齐
                    sz = (item->GetSize() / 4 + ((item->GetSize() % 4) != 0)) * 4;
                }
            }
            base->SetSize(base->GetSize() - sz);
            //std::cout << item->GetClassName() << " " << sz << " " << item->GetSize() << std::endl;
        }

        //
        m_baseList = m_baseList + baseClass->GetBaseClasses();
        //check virtual inherit
        std::unordered_set<std::string> us;
        for(auto item : m_baseList.GetBaseClasses())
        {
            //parent class showed twice or more
            if(us.find(item->Name()) != us.end()) 
            {
                if(item->GetInheritType() == VirtualType::NONVIRTUAL) throw std::runtime_error("inherit from grandpa class twice");
            }
            us.insert(item->Name());
        }
        m_propList = m_propList + props;
        m_fieldList = m_fieldList + fields;
        m_funcList = m_funcList + methods;
        int cnt = 0;
        us.clear();
        for(int i=0;i<m_propList.size();++i)
        {
            if(us.find(m_propList[i]->GetClassName() + "::" + m_propList[i]->Name()) != us.end()) continue;
            us.insert(m_propList[i]->GetClassName() + "::" + m_propList[i]->Name());
            m_propList[cnt++] = m_propList[i];
        }
        m_propList.resize(cnt);
        us.clear();
        cnt = 0;
        for(int i=0;i<m_fieldList.size();++i)
        {
            if(us.find(m_fieldList[i]->GetClassName() + "::" + m_fieldList[i]->Name()) != us.end()) continue;
            us.insert(m_fieldList[i]->GetClassName() + "::" + m_fieldList[i]->Name());
            m_fieldList[cnt++] = m_fieldList[i];
        }    
        m_fieldList.resize(cnt);  

        us.clear();
        cnt = 0;
        for(int i=0;i<m_funcList.size();++i)
        {
            if(us.find(m_funcList[i]->GetClassName() + "::" + m_funcList[i]->Name()) != us.end()) continue;
            us.insert(m_funcList[i]->GetClassName() + "::" + m_funcList[i]->Name());
            m_funcList[cnt++] = m_funcList[i];
        }    
        m_funcList.resize(cnt);
    }
    PropertyList m_propList;
    FieldList m_fieldList;
    std::shared_ptr<MemberInfo> m_destructor;
    //基类信息
    BaseList m_baseList;
    MethodList m_funcList;
    ConstructorList m_constructorList;
    static std::string m_name;
    VirtualType m_virt;
};

template <typename ClassT_>
std::string Type<ClassT_>::m_name = Type2String<ClassT_>();

template <typename T>
static std::shared_ptr<MemberInfo> AsMemberInfo(Type<T>&& item)
{
    std::shared_ptr<Type<T> > tmp = std::make_shared<Type<T> >();
    *tmp = item;
    return std::static_pointer_cast<MemberInfo>(tmp);
}

#define typeof(CLASS) Type<CLASS>().Register()

//实现RTTI需要提前注册
#define REGISTERRTTICLASS(CLASS) \
    FactoryInstance().insert(std::make_pair<std::string, std::shared_ptr<MemberInfo> >(std::string(typeid(CLASS).name()), AsMemberInfo(typeof(CLASS))))

}