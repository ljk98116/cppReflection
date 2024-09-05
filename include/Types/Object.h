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
        return m_data;
    }
private:
    void *m_data;
    std::shared_ptr<MemberInfo> m_typeInfo; 
};

template <typename T>
T Object::GetData() const
{
    return *reinterpret_cast<T*>(m_data);
}

template <typename T>
typename T::ClassT 
Object::GetData(T&& data) const
{
    return *reinterpret_cast<typename T::ClassT*>(m_data);
}

template <typename T>
typename T::ClassT
Object::GetData(const T& data) const
{
    return *reinterpret_cast<typename T::ClassT*>(m_data);
}

template <typename T>
void Object::SetData(const T& data)
{
    delete reinterpret_cast<T*>(m_data);
    m_data = new T;
    *((T*)m_data) = data;        
}

template <typename T>
Object::Object(T data)
{
    m_data = new T;
    *((T*)m_data) = data;
    auto typeInfo = Type<T>().Register();
    std::shared_ptr<decltype(typeInfo)> childPtr = std::make_shared<decltype(typeInfo)>();
    *childPtr = typeInfo;
    m_typeInfo = std::static_pointer_cast<MemberInfo>(childPtr);
}
}