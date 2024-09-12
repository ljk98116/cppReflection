#pragma once

namespace Reflection
{

class BaseList
{
public:
    BaseList()=default;
    explicit BaseList(std::vector<std::shared_ptr<MemberInfo> > members) :
    m_base(members)
    {}
    template <typename BaseInfo_>
    void AddBaseClass(BaseInfo_* base)
    {
        auto ptr = std::shared_ptr<MemberInfo>(base);
        m_base.push_back(ptr);
    }
    std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name)
    {
        for(auto base : m_base)
        {
            if(base->Name() == name) return base;
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<MemberInfo> > GetBaseClasses() const
    {
        return m_base;
    }
    auto operator+(const std::vector<std::shared_ptr<MemberInfo> >& rhs)
    {
        for(auto item : rhs)
        {
            m_base.push_back(item);
        }
        return *this;
    }
private:
    //Base<T>
    std::vector<std::shared_ptr<MemberInfo> > m_base;
};  

}