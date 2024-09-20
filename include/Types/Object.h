#pragma once
#include "traits.h"
#include <memory>

#include "MemberInfo.h"
#include "Type.h"
#include "TypeFactory.h"

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

    template <typename T>
    operator T()
    {
        try
        {
            return GetData<T>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return *(T*)nullptr;
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
    //解决RTTI问题，需要替代对应的虚函数
    constexpr bool is_pointer = std::is_pointer_v<T>;
    constexpr bool is_ref = std::is_reference_v<T>;

    using RawType = std::conditional_t<
                    is_ref,
                    std::conditional_t<
                        is_pointer,
                        std::remove_pointer_t<std::remove_reference_t<T> >,
                        std::remove_reference_t<T>
                    >,              
                    std::conditional_t<
                        is_pointer,
                        std::remove_pointer_t<T>,
                        T
                    >
                    >;
    
    std::shared_ptr<MemberInfo> derivedTypeInfo = nullptr;
    if constexpr (is_pointer) derivedTypeInfo = FactoryInstance()[typeid(*data).name()];
    else derivedTypeInfo = FactoryInstance()[typeid(data).name()];

    m_data = std::shared_ptr<void>(new RawType);
    if constexpr (is_pointer) *((RawType*)m_data.get()) = *data;
    else *((RawType*)m_data.get()) = data;

    auto typeInfo = Type<RawType>().Register();
    if constexpr (is_pointer || is_ref)
    {
        //使用derivedTypeInfo中的method信息替换typeInfo中虚函数的对应项
        for(int i=0;i<typeInfo.GetMethods().size();++i)
        {
            //基类虚函数重写为派生类的对应函数
            if(typeInfo.GetMethods()[i]->GetVirtualType() == VirtualType::VIRTUAL)
            {
                for(auto method_ : derivedTypeInfo->GetMethods())
                {
                    if(method_->Name() == typeInfo.GetMethods()[i]->Name() && method_->GetClassName() == derivedTypeInfo->GetClassName())
                    {
                        typeInfo.GetMethods()[i].swap(method_);
                    }
                }
            }
        }
    }
    std::shared_ptr<decltype(typeInfo)> childPtr = std::make_shared<decltype(typeInfo)>();
    *childPtr = typeInfo;
    m_typeInfo = std::static_pointer_cast<MemberInfo>(childPtr);
}

}