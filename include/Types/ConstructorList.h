#pragma once

#include <tuple>
#include <memory>
#include "MemberInfo.h"
#include <algorithm>

namespace Reflection
{

class ConstructorList
{
public:
    ConstructorList()=default;
    explicit ConstructorList(std::vector<std::shared_ptr<MemberInfo> > members) :
    m_methods(members)
    {}
    template <typename ConstructorInfo_>
    void AddMethod(ConstructorInfo_* prop)
    {
        auto ptr = std::shared_ptr<MemberInfo>(prop);
        m_methods.push_back(ptr);
    }
    std::shared_ptr<MemberInfo> GetConstructor(const std::vector<std::string>& arg_vec)
    {
        for(auto& method : m_methods)
        {
            if(method->GetArg() == arg_vec) return method;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<MemberInfo> >& GetConstructors()
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