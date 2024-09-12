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
    std::vector<std::shared_ptr<MemberInfo> > GetFields() const
    {
        return m_fields;
    }

    auto operator+(const std::vector<std::shared_ptr<MemberInfo> >& rhs)
    {
        for(auto item : rhs)
        {
            m_fields.push_back(item);
        }
        return *this;
    }
private:
    std::vector<std::shared_ptr<MemberInfo> > m_fields;
};    
 
}