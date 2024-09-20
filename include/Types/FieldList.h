#pragma once

#include <tuple>
#include <memory>
#include "MemberInfo.h"

namespace Reflection
{

class FieldList
{
public:
    FieldList()=default;
    explicit FieldList(std::vector<std::shared_ptr<MemberInfo> > members) :
    m_fields(members)
    {}
    template <typename FieldInfo_>
    void AddField(FieldInfo_* prop)
    {
        auto ptr = std::shared_ptr<MemberInfo>(prop);
        m_fields.push_back(ptr);
    }
    std::shared_ptr<MemberInfo> GetField(const std::string& name)
    {
        for(auto prop : m_fields)
        {
            if(prop->Name() == name) return prop;
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<MemberInfo> >& GetFields()
    {
        return m_fields;
    }
    size_t size() const {return m_fields.size();}
    auto operator+(const std::vector<std::shared_ptr<MemberInfo> >& rhs)
    {
        for(auto item : rhs)
        {
            m_fields.push_back(item);
        }
        return *this;
    }
    void push_back(std::shared_ptr<MemberInfo> field)
    {
        m_fields.push_back(field);
    }
    void resize(size_t sz)
    {
        m_fields.resize(sz);
    }
    std::shared_ptr<MemberInfo>& operator[](size_t idx)
    {
        return m_fields[idx];
    }
private:
    std::vector<std::shared_ptr<MemberInfo> > m_fields;
};    
 
}