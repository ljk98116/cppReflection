#pragma once

#include <tuple>
#include <memory>
#include "MemberInfo.h"

namespace Reflection
{
class MethodList
{
public:
    MethodList()=default;
    explicit MethodList(std::vector<std::shared_ptr<MemberInfo> > members) :
    m_methods(members)
    {}
    template <typename MethodInfo_>
    void AddMethod(MethodInfo_* prop)
    {
        auto ptr = std::shared_ptr<MemberInfo>(prop);
        m_methods.push_back(ptr);
    }
    std::shared_ptr<MemberInfo> GetMethod(const std::string& name)
    {
        for(auto prop : m_methods)
        {
            if(prop->Name() == name) return prop;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<MemberInfo> > GetMethods() const
    {
        return m_methods;
    }

    size_t size() const {return m_methods.size();}
    auto operator+(const std::vector<std::shared_ptr<MemberInfo> >& rhs)
    {
        for(auto item : rhs)
        {
            m_methods.push_back(item);
        }
        return *this;
    }
    void push_back(std::shared_ptr<MemberInfo> field)
    {
        m_methods.push_back(field);
    }
    void resize(size_t sz)
    {
        m_methods.resize(sz);
    }
    std::shared_ptr<MemberInfo>& operator[](size_t idx)
    {
        return m_methods[idx];
    }
private:
    std::vector<std::shared_ptr<MemberInfo> > m_methods;
};    
}