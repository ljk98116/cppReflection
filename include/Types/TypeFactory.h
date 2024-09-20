#pragma once

#include <include/Types/MemberInfo.h>
#include <unordered_map>
#include <mutex>

//存储所有已注册的Type<T>的全局单例，需要考虑线程安全，以typeid(T).name()为索引的哈希表
//object在接收到指针或者引用后，利用RTTI找到所指对象的typeinfo，修改其中基类的虚函数项为子类中的对应项
namespace Reflection
{

class TypeFactory
{
public:
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
private:
    std::unordered_map<std::string, std::shared_ptr<MemberInfo> > m_typeDict;
    std::mutex mut;
};

TypeFactory& FactoryInstance();
}