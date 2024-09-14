#pragma once
#include <iostream>
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
        m_base.push_back(std::shared_ptr<MemberInfo>(base));
        int i = 1;
        while(m_base.size() - i > 0 && m_base[m_base.size() - i - 1]->GetInheritType() == VirtualType::VIRTUAL)
        {
            m_base[m_base.size() - i - 1].swap(m_base[m_base.size() - i]);
            ++i; 
        }
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
    size_t size() const {return m_base.size();}
    auto operator+(const std::vector<std::shared_ptr<MemberInfo> >& rhs)
    {
        for(auto item : rhs)
        {
            m_base.push_back(item);
            int i = 1;
            while(m_base.size() - i > 0 && m_base[m_base.size() - i - 1]->GetInheritType() == VirtualType::VIRTUAL)
            {
                m_base[m_base.size() - i - 1].swap(m_base[m_base.size() - i]);
                ++i; 
            }            
        }
        return *this;
    }
    void push_back(std::shared_ptr<MemberInfo> ptr)
    {
        m_base.push_back(std::shared_ptr<MemberInfo>(ptr));
        int i = 1;
        while(m_base.size() - i > 0 && m_base[m_base.size() - i - 1]->GetInheritType() == VirtualType::VIRTUAL)
        {
            m_base[m_base.size() - i - 1].swap(m_base[m_base.size() - i]);
            ++i; 
        } 
    }
    void resize(size_t sz)
    {
        m_base.resize(sz);
    }
    std::shared_ptr<MemberInfo>& operator[](size_t idx)
    {
        return m_base[idx];
    }
private:
    //Base<T>
    std::vector<std::shared_ptr<MemberInfo> > m_base;
};  

}