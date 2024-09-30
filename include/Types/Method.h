#pragma once

#include <memory>
#include <functional>
#include <type_traits>
#include <variant>

#include <include/Types/Object.h>
#include <include/Types/MemberInfo.h>
#include <include/Types/traits.h>

namespace Reflection
{

template <typename Tuple, size_t ...Is>
auto TransformArgs_(std::index_sequence<Is...>, std::vector<Object>& vec, Tuple&& t)
{
    auto caster = CastTypes<std::tuple_element_t<Is, std::decay_t<Tuple> >... >{};
    return caster(vec[Is]...);
}

template <typename Tuple>
auto TransformArgs(std::vector<std::pair<std::string, Object>>& vec, Tuple&& t)
{
    std::vector<Object> vec_;
    for(auto& item : vec) vec_.push_back(item.second);

    return TransformArgs_(std::make_index_sequence<std::tuple_size_v<Tuple> >{}, vec_, t);
}

template <typename Tuple>
auto TransformObjectArgs(std::vector<Object>& vec, Tuple&& t)
{
    return TransformArgs_(std::make_index_sequence<std::tuple_size_v<Tuple> >{}, vec, t);
}
template <typename Ret, typename ...Args>
class NormalMethodInfo : public MemberInfo
{
public:
    using FuncT = std::function<Ret(Args...)>;
    using ArgsT = std::tuple<Args...>;
    using RetT = Ret;

    NormalMethodInfo(
        std::string name, 
        Ret(*func)(Args...), 
        AccessType access, 
        StaticType staticType, 
        VirtualType virt, 
        FuncType funcType,
        std::vector<std::pair<std::string, Object>> default_args
    ):
    m_name(name), m_func(func), m_access(access), m_static(staticType), m_virtual(virt), m_funcType(funcType),
    m_default_args(default_args)
    {
        if constexpr (sizeof...(Args) > 0) init_args<Args...>();
    }

    std::string Name() const override
    {
        return m_name;
    }

    AccessType GetAccess() const override
    {
        return m_access;
    }

    StaticType GetStaticType() const override
    {
        return m_static;
    }

    VirtualType GetVirtualType() const override
    {
        return m_virtual;
    }

    std::string GetClassName() override
    {
        return "";
    }

    template <typename ...Rest>
    Object invoke(Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {
            if(m_access == AccessType::PUBLIC)
            {
                if constexpr (std::is_same<RetT, void>::value)
                {
                    m_func(std::forward<Args>(Args(args))...);
                    return Object(nullptr);
                }
                else return m_func(std::forward<Args>(Args(args))...);
            }
            throw std::runtime_error("can not access");
        }
        else
        {
            const size_t n = sizeof...(Rest);
            assert(n + m_default_args.size() == sizeof...(Args));
            if(m_access == AccessType::PUBLIC)
            {
                //截取类型序列
                using RestArgs = SequenceSuffix<n, Args...>;
                using FrontArgs = SequencePrefix<n, Args...>;
                //利用类型元组转化default_args_vector，输出新的元组
                auto default_args_tuple = TransformArgs(m_default_args, RestArgs{});
                auto front_args_vec = std::vector<Object>{std::forward<Object>(args)...};

                auto pass_args_tuple = TransformObjectArgs(front_args_vec, FrontArgs{});
                auto args_tuple = std::tuple_cat(pass_args_tuple, default_args_tuple);

                using suffix_seq = std::make_index_sequence<std::tuple_size<RestArgs>::value>;
                if constexpr (std::is_same<RetT, void>::value)
                {
                    callFunc(m_func, args_tuple);
                    return Object(nullptr);                    
                }       
                else
                {
                    return callFunc(m_func, args_tuple);
                }
            }
            throw std::runtime_error("can not access");            
        }
        if constexpr (std::is_same<RetT, void>::value) return Object(nullptr);
        else return Object(nullptr);
    }

    Object Invoke(Object obj1) override
    {
        return invoke(obj1);
    }

    Object Invoke(Object obj1, Object obj2) override
    {
        return invoke(obj1, obj2);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3) override
    {
        return invoke(obj1, obj2, obj3);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4) override
    {
        return invoke(obj1, obj2, obj3, obj4);
    }
    //more params
    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5, Object obj6) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5, obj6);
    }
    
    std::vector<std::string>& GetArg() override
    {
        return m_args;
    }
private:
    template <typename T, typename ...Rest>
    void init_args()
    {
        m_args.push_back(Type2String<T>());
        if constexpr (sizeof...(Rest)) init_args<Rest...>();
    }
    template <typename F, typename Tuple, size_t ...Is>
    RetT callFunc_(std::index_sequence<Is...>, F&& func, Tuple& t)
    {
        return std::forward<F>(func)(std::get<Is>(t)...);
    }
    template <typename F, typename ...T>
    RetT callFunc(F&& func, std::tuple<T...>& t)
    {
        return callFunc_(std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
    }
    FuncT m_func;
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
    std::vector<std::string> m_args;
    std::vector<std::pair<std::string, Object>> m_default_args;
};

template <typename Ret, typename Class, typename ...Args>
class MemberMethodInfo : public MemberInfo
{
public:
    using MemberFuncT = Ret(Class::*)(Args...);
    using StaticMemberFuncT = Ret(*)(Args...);
    using ArgsT = std::tuple<Args...>;
    using RetT = Ret;
    using ClassT = Class;

    MemberMethodInfo(
        std::string name, 
        Ret(Class::*func)(Args...), 
        AccessType access, 
        StaticType staticType, 
        VirtualType virt, 
        FuncType funcType,
        std::vector<std::pair<std::string, Object>> default_args
    ):
    m_name(init_method_name(name)), m_access(access), 
    m_static(staticType), m_virtual(virt), 
    m_funcType(funcType), m_func(func),
    m_default_args(default_args)
    {
        if constexpr (sizeof...(Args) > 0) init_args<Args...>();
    }

    std::string Name() const override
    {
        return m_name;
    }

    AccessType GetAccess() const override
    {
        return m_access;
    }

    StaticType GetStaticType() const override
    {
        return m_static;
    }

    VirtualType GetVirtualType() const override
    {
        return m_virtual;
    }

    std::string GetClassName() override
    {
        return Type2String<ClassT>();   
    }

    std::shared_ptr<MemberInfo> GetBaseClass(const std::string& name) override
    {
        auto typeInfo = typeof(ClassT);
        return typeInfo.GetBaseClass(name);
    }

    std::vector<std::string>& GetArg() override
    {
        return m_args;
    }

    //注意基类指针、引用调用子类虚方法时的情况，需要进行RTTI判断
    template <typename T, typename ...Rest>
    Object invoke(T&& obj, Rest&&... args)
    {
        if constexpr (sizeof...(Rest) == sizeof...(Args))
        {            
            if constexpr (std::is_same<RetT, void>::value)
            {
                if(obj.IsPointer() && *(ClassT**)(obj.GetRawData()) != nullptr) ((*(ClassT**)(obj.GetRawData()))->*m_func)(std::forward<Args>(Args(args))...);
                else if(obj.IsRef()) (((std::reference_wrapper<ClassT>)(obj)).get().*m_func)(std::forward<Args>(Args(args))...);
                else
                {
                    if constexpr (std::is_abstract_v<ClassT>) throw std::runtime_error("abstract cast");
                    else ((ClassT)(obj).*m_func)(std::forward<Args>(Args(args))...);
                }
                return Object(nullptr);
            }
            else
            {
                if(obj.IsPointer() && *(ClassT**)(obj.GetRawData()) != nullptr) return ((*(ClassT**)(obj.GetRawData()))->*m_func)(std::forward<Args>(Args(args))...);
                else if(obj.IsRef()) return (((std::reference_wrapper<ClassT>)(obj)).get().*m_func)(std::forward<Args>(Args(args))...);
                else
                {
                    if constexpr (std::is_abstract_v<ClassT>) throw std::runtime_error("abstract cast");
                    else return ((ClassT)(obj).*m_func)(std::forward<Args>(Args(args))...);                    
                }              
            }
        }
        else
        {
            const int n = sizeof...(Rest);
            assert(n + m_default_args.size() == sizeof...(Args));
            if(m_access == AccessType::PUBLIC)
            {
                //截取类型序列
                using RestArgs = SequenceSuffix<n, Args...>;
                using FrontArgs = SequencePrefix<n, Args...>;
                //利用类型元组转化default_args_vector，输出新的元组
                auto default_args_tuple = TransformArgs(m_default_args, RestArgs{});
                auto front_args_vec = std::vector<Object>{std::forward<Object>(args)...};

                auto pass_args_tuple = TransformObjectArgs(front_args_vec, FrontArgs{});
                auto args_tuple = std::tuple_cat(pass_args_tuple, default_args_tuple);

                using suffix_seq = std::make_index_sequence<std::tuple_size<RestArgs>::value>;
                if constexpr (std::is_same<RetT, void>::value)
                {
                    if(obj.IsPointer() && *(ClassT**)(obj.GetRawData()) != nullptr)
                    {
                        auto ptr = (*(ClassT**)(obj.GetRawData()));
                        callFunc(ptr, m_func, args_tuple);
                    }
                    else if(obj.IsRef())
                    {
                        auto ref = (std::reference_wrapper<ClassT>)(obj);
                        callFunc(ref, m_func, args_tuple);
                    }
                    else
                    {
                        if constexpr (std::is_abstract_v<ClassT>) throw std::runtime_error("abstract cast");
                        else
                        {
                            callFunc((ClassT)(obj), m_func, args_tuple);
                        }
                    }
                    return Object(nullptr);                   
                }       
                else
                {
                    if(obj.IsPointer() && *(ClassT**)(obj.GetRawData()) != nullptr)
                    {
                        auto ptr = (*(ClassT**)(obj.GetRawData()));
                        return callFunc(ptr, m_func, args_tuple);
                    }
                    else if(obj.IsRef())
                    {
                        auto ref = (std::reference_wrapper<ClassT>)(obj);
                        return callFunc(ref, m_func, args_tuple);
                    }
                    else
                    {
                        if constexpr (std::is_abstract_v<ClassT>) throw std::runtime_error("abstract cast");
                        else
                        {
                            return callFunc((ClassT)(obj), m_func, args_tuple);
                        }
                    }
                    return Object(nullptr); 
                }
            }
            throw std::runtime_error("can not access");            
        }
        return Object(nullptr);
    }

    Object Invoke(Object obj1) override
    {
        return invoke(obj1);
    }

    Object Invoke(Object obj1, Object obj2) override
    {
        return invoke(obj1, obj2);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3) override
    {
        return invoke(obj1, obj2, obj3);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4) override
    {
        return invoke(obj1, obj2, obj3, obj4);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5);
    }

    Object Invoke(Object obj1, Object obj2, Object obj3, Object obj4, Object obj5, Object obj6) override
    {
        return invoke(obj1, obj2, obj3, obj4, obj5, obj6);
    }
    //more params

private:
    template <typename T, typename ...Rest>
    std::string ParseArgsImpl()
    {
        std::string ret = Type2String<T>();
        if constexpr (sizeof...(Rest) == 0) return ret;
        else
        {
            ret += ",";
            return ret + ParseArgsImpl<Rest...>();
        }
    }
    template <typename ...T>
    std::string ParseArgs()
    {
        if constexpr (sizeof...(T) == 0) return "";
        else return ParseArgsImpl<T...>();
    }

    std::string init_method_name(const std::string& name)
    {
        if constexpr (sizeof...(Args) == 0) return name;
        else
        {
            return name + "(" + ParseArgs<Args...>() + ")";
        }
    }

    template <typename T, typename ...Rest>
    void init_args()
    {
        m_args.push_back(Type2String<T>());
        if constexpr (sizeof...(Rest)) init_args<Rest...>();
    }

    //ClassT&&, ClassT*, std::reference_wrapper<T>
    template <typename F, typename Tuple, size_t ...Is>
    RetT callFunc_(ClassT&& obj, std::index_sequence<Is...>, F&& func, Tuple& t)
    {
        return (obj.*std::forward<F>(func))(std::get<Is>(t)...);
    }

    template <typename F, typename ...T>
    RetT callFunc(ClassT&& obj, F&& func, std::tuple<T...>& t)
    {
        if constexpr (std::is_same_v<RetT, void>)
        {
            callFunc_(std::forward<ClassT>(obj), std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
        }
        else return callFunc_(std::forward<ClassT>(obj), std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
    }

    template <typename F, typename Tuple, size_t ...Is>
    RetT callFunc_(ClassT* obj, std::index_sequence<Is...>, F&& func, Tuple& t)
    {
        if constexpr (std::is_same_v<RetT, void>) (obj->*std::forward<F>(func))(std::get<Is>(t)...);
        else return (obj->*std::forward<F>(func))(std::get<Is>(t)...);
    }

    template <typename F, typename ...T>
    RetT callFunc(ClassT* obj, F&& func, std::tuple<T...>& t)
    {
        if constexpr (std::is_same_v<RetT, void>) callFunc_(obj, std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
        else return callFunc_(obj, std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
    }
    template <typename F, typename Tuple, size_t ...Is>
    RetT callFunc_(std::reference_wrapper<ClassT> obj, std::index_sequence<Is...>, F&& func, Tuple& t)
    {
        if constexpr (std::is_same_v<RetT, void>) (obj.get().*std::forward<F>(func))(std::get<Is>(t)...);
        else return (obj.get().*std::forward<F>(func))(std::get<Is>(t)...);
    }
    template <typename F, typename ...T>
    RetT callFunc(std::reference_wrapper<ClassT> obj, F&& func, std::tuple<T...>& t)
    {
        if constexpr (std::is_same_v<RetT, void>) callFunc_(std::forward<std::reference_wrapper<ClassT> >(obj), std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
        else return callFunc_(std::forward<std::reference_wrapper<ClassT> >(obj), std::make_index_sequence<sizeof...(T)>{}, std::forward<F>(func), t);
    } 
    MemberFuncT m_func;
    std::string m_name;
    AccessType m_access;
    StaticType m_static;
    VirtualType m_virtual;
    FuncType m_funcType;
    std::vector<std::string> m_args;
    std::vector<std::pair<std::string, Object>> m_default_args;
};

#define ARGS(...) __VA_ARGS__

#define DEFAULT_ARG(ARG, VALUE) std::pair<std::string, Object>(#ARG, Object(VALUE))

#define NORMALMETHOD(RET, NAME, ARGS, STATICFLAG, ...) \
    new NormalMethodInfo( \
        #NAME, \
        (RET(*)(ARGS))&NAME, \
        AccessType::PUBLIC, \
        StaticType::STATICFLAG, \
        VirtualType::NONVIRTUAL, \
        FuncType::Normal, \
        std::vector<std::pair<std::string, Object>>{__VA_ARGS__} \
    )    

#define NORMALMEMBERMETHOD(RET, CLASS, NAME, ARGS, ACCESS, STATICFLAG, VIRTUALFLAG, ...) \
    new MemberMethodInfo(#NAME, \
        (RET(CLASS::*)(ARGS))&CLASS::NAME, \
        AccessType::ACCESS, \
        StaticType::STATICFLAG, \
        VirtualType::VIRTUALFLAG, \
        FuncType::Member, \
        std::vector<std::pair<std::string, Object>>{__VA_ARGS__}\
    )

#define STATICMEMBERMETHOD(RET, CLASS, NAME, ARGS, ACCESS, ...) \
    new NormalMethodInfo(#NAME, \
    (RET(*)(ARGS))&CLASS::NAME, \
    AccessType::ACCESS, \
    StaticType::STATIC, \
    VirtualType::NONVIRTUAL, \
    FuncType::Member, \
    std::vector<std::pair<std::string, Object>>{__VA_ARGS__} \
    )
}