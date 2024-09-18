#pragma once
#include "traits.h"
#include <memory>

#include "MemberInfo.h"
#include "Type.h"

namespace Reflection
{
//确保更改Object的值时重新分配空间
class Object
{
public:
    template <typename T>
    Object(T data);

    ~Object();

    template <typename T>
    T GetData() const;

    template <typename T>
    typename T::ClassT GetData(T&& data) const;

    template <typename T>
    typename T::ClassT GetData(const T&) const;

    template <typename T>
    void SetData(const T& data);  

    void *GetRawData()
    {
        return m_data.get();
    }

    std::shared_ptr<void>& Data()
    {
        return m_data;
    }
    auto GetTypeInfo() const
    {
        return m_typeInfo;
    }
private:
    std::shared_ptr<void> m_data;
    //具体数据的Type<T>
    std::shared_ptr<MemberInfo> m_typeInfo; 
};

template <typename T>
T Object::GetData() const
{
    return *reinterpret_cast<T*>(m_data.get());
}

template <typename T>
typename T::ClassT 
Object::GetData(T&& data) const
{
    if(m_data == nullptr) throw std::runtime_error("object is empty");
    return *reinterpret_cast<typename T::ClassT*>(m_data.get());
}

template <typename T>
typename T::ClassT
Object::GetData(const T& data) const
{
    if(m_data == nullptr) throw std::runtime_error("object is empty");
    return *reinterpret_cast<typename T::ClassT*>(m_data.get());
}

template <typename T>
void Object::SetData(const T& data)
{
    m_data.reset();
    m_data = std::shared_ptr<void>(new T);
    *((T*)m_data.get()) = data;        
}

template <typename T>
Object::Object(T data)
{
    m_data = std::shared_ptr<void>(new T);
    *((T*)m_data.get()) = data; 
    auto typeInfo = Type<T>().Register();
    std::shared_ptr<decltype(typeInfo)> childPtr = std::make_shared<decltype(typeInfo)>();
    *childPtr = typeInfo;
    m_typeInfo = std::static_pointer_cast<MemberInfo>(childPtr);
}


}