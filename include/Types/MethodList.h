#pragma once

#include <tuple>
#include <memory>
#include "MemberInfo.h"
#include <algorithm>

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
    std::shared_ptr<MemberInfo> GetMethod(const std::string& name, const std::string& className)
    {
        bool is_pointer = std::find(className.begin(), className.end(), '*') != className.end();
        bool is_ref = std::find(className.begin(), className.end(), '&') != className.end();
        auto className_ = className.substr(0, className.size() - 1);
        //需要看是否构成虚函数重写的情况
        //看prop是否有重名的虚方法
        bool has_virtual = false;
        for(auto& prop : m_methods)
        {
            if(prop->Name() == name && prop->GetVirtualType() == VirtualType::VIRTUAL) 
            {
                has_virtual = true;
                break;
            }
        }
        for(auto& prop : m_methods)
        {
            //同名
            if(prop->Name() == name)
            {
                if(!((is_pointer || is_ref) && has_virtual) ) return prop;
                //如果子类对应，返回对应方法
                else 
                {
                    if(prop->GetClassName() == className_) return prop;
                }
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<MemberInfo> >& GetMethods()
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