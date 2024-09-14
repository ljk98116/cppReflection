#include <bits/stdc++.h>
#include <Reflection.h>

using namespace std;
using namespace Reflection;

class A
{
public:
    A(){}
    static auto Register()
    {
        return Type<A>().AddProperty(PROPERTYDEFAULT(A, a, ATTR_A, PUBLIC, NONE));
    }
private:
    int a;
};

//B共有继承A，B中b_str1不能被C访问，b_str2可以
class B : public A
{
public:
    B(){}
    virtual ~B(){}
    static auto Register()
    {
        return Type<B>().AddBaseClass(BASE(A, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddProperty(PROPERTYDEFAULT(B, b_str1, B_STR1, PRIVATE, NONE))
        .AddProperty(PROPERTYDEFAULT(B, b_str2, B_STR2, PROTECT, NONE));
    }
private:
    std::string b_str1;
    std::string b_str2;
};

class C : protected B
{
public:
    C(){}
    virtual ~C(){}
    static auto Register()
    {
        return Type<C>().AddBaseClass(BASE(B, PROTECT, VIRTUAL, NONVIRTUAL));
    }
private:

};

class C1
{
public:
    C1(){}
    ~C1(){}
    int Val() const {return x;}
    static auto Register()
    {
        return Type<C1>().AddProperty(PROPERTYDEFAULT(C1, x, X, PUBLIC, NONE));
    }
private:
    int x;
};

class C2 : virtual public C1
{
public:
    C2(){}
    virtual ~C2(){}
    std::string s;
    static auto Register()
    {
        return Type<C2>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(C1, PUBLIC, VIRTUAL, VIRTUAL))
        .AddField(FIELD(C2, s, PUBLIC, NONE))
        .AddProperty(PROPERTYDEFAULT(C2, y, Y, PUBLIC, NONE));
    }
private:
    int y;
};

class C3 : virtual public C1
{
public:
    C3(){}
    virtual ~C3(){}
    static auto Register()
    {
        return Type<C3>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(C1, PUBLIC, VIRTUAL, VIRTUAL));
    }
};

class C4 : public C2, public C3
{
public:
    C4(){}
    virtual ~C4(){}
    static auto Register()
    {
        return Type<C4>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(C2, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddBaseClass(BASE(C3, PUBLIC, VIRTUAL, NONVIRTUAL));
    }
};

int main()
{
    auto typeInfo = typeof(C);

    auto prop_a = typeInfo.GetProperty("ATTR_A");
    auto prop_bstr1 = typeInfo.GetProperty("B_STR1");
    auto prop_bstr2 = typeInfo.GetProperty("B_STR2");

    cout << AccessType2String(prop_a->GetAccess()) << endl;
    cout << AccessType2String(prop_bstr1->GetAccess()) << endl;
    cout << AccessType2String(prop_bstr2->GetAccess()) << endl;

    Object c(C{});
    //prop_a->InvokeSet(c, 56); # protected denied
    //prop_bstr1->InvokeSet(c, "error"); # private denied
    //prop_bstr2->InvokeSet(c, "protected"); # protected denied

    //多重继承，如果存在虚拟继承，属性指针需要加上16的偏移, 2个虚表指针大小
    auto t = typeof(C4);//if C2 not inherit C1 with virtual runtime error expected
    Object x(C4{});
    auto prop_x = t.GetProperty("X");
    prop_x->InvokeSet(x, 14);
    cout << x.GetData(t).Val() << endl;
    return 0;
}