#pragma once

#include <tuple>
#include <memory>
#include "MemberInfo.h"

namespace Reflection
{

class EnumList
{
public:
    EnumList()=default;
    explicit EnumList(std::vector<std::shared_ptr<MemberInfo> > members) :
    m_Enums(members)
    {}
    template <typename EnumInfo_>
    void AddEnum(EnumInfo_* prop)
    {
        auto ptr = std::shared_ptr<MemberInfo>(prop);
        m_Enums.push_back(ptr);
    }
    std::shared_ptr<MemberInfo> GetEnum(const std::string& name)
    {
        for(auto prop : m_Enums)
        {
            if(prop->Name() == name) return prop;
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<MemberInfo> >& GetEnums()
    {
        return m_Enums;
    }
    size_t size() const {return m_Enums.size();}
    auto operator+(const std::vector<std::shared_ptr<MemberInfo> >& rhs)
    {
        for(auto item : rhs)
        {
            m_Enums.push_back(item);
        }
        return *this;
    }
    void push_back(std::shared_ptr<MemberInfo> Enum)
    {
        m_Enums.push_back(Enum);
    }
    void resize(size_t sz)
    {
        m_Enums.resize(sz);
    }
    std::shared_ptr<MemberInfo>& operator[](size_t idx)
    {
        return m_Enums[idx];
    }
private:
    std::vector<std::shared_ptr<MemberInfo> > m_Enums;
};    
 
}