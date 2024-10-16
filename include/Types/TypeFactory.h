#pragma once

#include <include/Types/MemberInfo.h>
#include <unordered_map>
#include <mutex>
#include <functional>
//存储所有已注册的Type<T>的全局单例，需要考虑线程安全，以typeid(T).name()为索引的哈希表
//object在接收到指针或者引用后，利用RTTI找到所指对象的typeinfo，修改其中基类的虚函数项为子类中的对应项
namespace Reflection
{

extern void(*Regfunc)();

class TypeRegister
{
public:
    explicit TypeRegister(void (*func)());
};

//另一个单例来赋值
class TypeFactory
{
public:
    TypeFactory()=default;
    void insert(std::pair<std::string, std::shared_ptr<MemberInfo> > item)
    {
        std::lock_guard<std::mutex> lk(mut);
        m_typeDict.insert(item);
    }
    bool HasKey(const std::string& name)
    {
        std::lock_guard<std::mutex> lk(mut);
        return m_typeDict.count(name);
    }
    std::shared_ptr<MemberInfo>& operator[](const std::string& name)
    {
        std::lock_guard<std::mutex> lk(mut);
        return m_typeDict[name];
    }
    template <typename MethodInfo_>
    void RegisterMethod(MethodInfo_ *method)
    {
        std::lock_guard<std::mutex> lk(mut);
        m_funcs.push_back(std::shared_ptr<MemberInfo>(method));
    }

    template <typename EnumInfo_>
    void RegisterEnum(EnumInfo_ *enumInfo)
    {
        std::lock_guard<std::mutex> lk(mut);
        m_enums.push_back(std::shared_ptr<MemberInfo>(enumInfo));
    }

    std::shared_ptr<MemberInfo> GetMethod(const std::string& name, const std::vector<std::string>& args)
    {
        std::lock_guard<std::mutex> lk(mut);
        for(auto& method : m_funcs)
        {
            if(method->Name() == name && method->GetArg() == args) return method;
        }
        return nullptr;
    }
    std::shared_ptr<MemberInfo> GetEnumType(const std::string& name)
    {
        std::lock_guard<std::mutex> lk(mut);
        for(auto& info : m_enums)
        {
            if(info->Name() == name) return info;
        }
        return nullptr;        
    }
private:
    std::unordered_map<std::string, std::shared_ptr<MemberInfo> > m_typeDict;
    std::vector<std::shared_ptr<MemberInfo> > m_funcs;
    std::vector<std::shared_ptr<MemberInfo> > m_enums;
    std::mutex mut;
};

TypeFactory& FactoryInstance();

template <typename MethodInfo_>
void RegisterMethod(MethodInfo_ *method)
{
    FactoryInstance().RegisterMethod(method);
}

template <typename EnumInfo_>
void RegisterEnum(EnumInfo_ *enumInfo)
{
    FactoryInstance().RegisterEnum(enumInfo);
}

#define TYPEREGISTER \
void __TypeRegisterFunc(); \
static TypeRegister TypeRegister___t123456789{&__TypeRegisterFunc}; \
void __TypeRegisterFunc()

}