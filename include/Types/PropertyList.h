#pragma once

#include <vector>
#include <memory>

#include "MemberInfo.h"

namespace Reflection
{
class PropertyList
{
public:
    PropertyList()=default;
    explicit PropertyList(std::vector<std::shared_ptr<MemberInfo> > members) :
    m_props(members)
    {}
    template <typename PropertyInfo_>
    void AddProperty(PropertyInfo_* prop)
    {
        auto ptr = std::shared_ptr<MemberInfo>(prop);
        m_props.push_back(ptr);
    }
    std::shared_ptr<MemberInfo> GetProperty(const std::string& name)
    {
        for(auto prop : m_props)
        {
            if(prop->Name() == name) return prop;
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<MemberInfo> > GetProperties() const
    {
        return m_props;
    }
private:
    std::vector<std::shared_ptr<MemberInfo> > m_props;
};

}

