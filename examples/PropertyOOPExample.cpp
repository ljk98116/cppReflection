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
        return Type<A>()
        .AddProperty(PROPERTYDEFAULT(A, a, ATTR_A, PUBLIC, NONE))
        .AddMethod(STATICMEMBERMETHOD(int, A, add, ARGS(int,int), PUBLIC));
    }
    static int add(int a, int b){return a+b;}
private:
    int a;
};

//B共有继承A，B中b_str1不能被C访问，b_str2可以
class B : public A
{
public:
    B(){}
    B(std::string s1, std::string s2):
    b_str1(s1), b_str2(s2)
    {}

    virtual ~B(){}
    static auto Register()
    {
        return Type<B>().AddBaseClass(BASE(A, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddDestructor(DESTRUCTOR(PUBLIC, VIRTUAL, B))
        .AddConstructor(CONSTRUCTOR(PUBLIC, B, string, string))
        .AddProperty(PROPERTYDEFAULT(B, b_str1, B_STR1, PUBLIC, NONE))
        .AddProperty(PROPERTYDEFAULT(B, b_str2, B_STR2, PUBLIC, NONE));
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
        return Type<C>()
        .AddBaseClass(BASE(B, PROTECT, VIRTUAL, NONVIRTUAL))
        .AddConstructor(CONSTRUCTOR(PUBLIC, C))
        .AddDestructor(DESTRUCTOR(PUBLIC, VIRTUAL, C));
    }
private:

};

class C1//4
{
public:
    C1(){}
    ~C1(){}
    int GetX() const {return x;}
    static auto Register()
    {
        return Type<C1>()
        .AddProperty(PROPERTYDEFAULT(C1, x, X, PUBLIC, NONE))
        .AddDestructor(DESTRUCTOR(PUBLIC, NONVIRTUAL, C1))
        .AddMethod(NORMALMEMBERMETHOD(int, C1, GetX, ARGS(), PUBLIC, NONE, NONVIRTUAL));
    }
private:
    int x=0;
    char c;
};

class C2 : virtual public C1//8 + 4 +
{
public:
    C2(){}
    virtual ~C2(){}
    std::string s;
    std::string Add(char c)
    {
        return s + c;
    }
    GET(Y, int)
    {
        return y;
    }
    SET(Y, int)
    {
        y = value;
    }
    static auto Register()
    {
        return Type<C2>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(C1, PUBLIC, NONVIRTUAL, VIRTUAL))
        .AddField(FIELD(C2, s, PUBLIC, NONE))
        .AddProperty(PROPERTY(C2, y, Y, PUBLIC, NONE))
        .AddMethod(NORMALMEMBERMETHOD(std::string, C2, Add, ARGS(char), PUBLIC, NONE, NONVIRTUAL));
    }
private:
    int y=0;
    char c;
};

class C3 : virtual public C1 //size 8
{
public:
    C3(){}
    virtual ~C3(){}
    static auto Register()
    {
        return Type<C3>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(C1, PUBLIC, NONVIRTUAL, VIRTUAL))
        .AddConstructor(CONSTRUCTOR(PUBLIC, C3))
        .AddDestructor(DESTRUCTOR(PUBLIC, VIRTUAL, C3));
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
        .AddBaseClass(BASE(C3, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddDestructor(DESTRUCTOR(PUBLIC, VIRTUAL, C4));
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
    cout << sizeof(C1) << " " << sizeof(C2) << " " << sizeof(C3) << " " << sizeof(string) << endl;
    auto t = typeof(C4);//if C2 not inherit C1 with virtual runtime error expected
    auto it = C4{};
    Object x(it);
    cout << it.getY() << endl;
    auto prop_x = t.GetProperty("X");
    auto prop_y = t.GetProperty("Y");
    prop_x->InvokeSet(x, 14);
    int x_value = prop_x->InvokeGet(x);
    cout << x_value << endl;
    prop_y->InvokeSet(x, 67);
    cout << x.GetData(t).getY() << endl;
    int y = prop_y->InvokeGet(x);
    cout << y << endl;

    auto addFuncInfo = t.GetMethod("Add(char)");
    auto ret = addFuncInfo->Invoke(x, 'o');
    cout << (std::string)ret << endl;

    auto addFuncInfo2 = typeInfo.GetMethod("add");
    auto ret2 = addFuncInfo2->Invoke(67, 89);
    cout << (int)ret2 << endl;

    auto binfo = typeof(B);
    auto constructorInfo = typeof(B).GetConstructor(ARGTYPE(string, string));
    auto ret3 = constructorInfo->Invoke(string("b1"), string("b2"));
    auto pb1 = binfo.GetProperty("B_STR1");
    auto xx = pb1->InvokeGet(ret3);
    cout << (string)(pb1->InvokeGet(ret3)) << endl;

    auto destructorInfo = t.GetDestructor();
    destructorInfo->Invoke(x);

    return 0;
}