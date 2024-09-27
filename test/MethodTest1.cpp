#include <bits/stdc++.h>
#include <Reflection.h>

using namespace std;
using namespace Reflection;

class A
{
public:
    virtual void display() = 0;
    static auto Register()
    {
        return Type<A>(VirtualType::VIRTUAL)
        .AddMethod(NORMALMEMBERMETHOD(void, A, display, ARGS(), PUBLIC, NONE, VIRTUAL));
    }
};

class B : public A
{
public:
    explicit B(int x):
    m_x(x)
    {}
    virtual ~B(){}
    void display() override
    {
        cout << "this is class B, x=" << m_x << endl;
    }
    void display(int x)
    {
        cout << "this is class B, display " << x << endl;
    }
    static auto Register()
    {
        return Type<B>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(A, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddConstructor(CONSTRUCTOR(PUBLIC, B, int))
        .AddMethod(NORMALMEMBERMETHOD(void, B, display, ARGS(int), PUBLIC, NONE, NONVIRTUAL))
        .AddMethod(NORMALMEMBERMETHOD(void, B, display, ARGS(), PUBLIC, NONE, NONVIRTUAL));
    }
private:
    int m_x;
};

int Sub(int x, int y)
{
    return x - y;
}

static double mult(int x, int y)
{
    return x * y;
}

int main()
{
    B b(9);
    A* ptr = &b;
    A& ref = b;
    ptr->display();
    REGISTERRTTICLASS(B);
    auto obj = Object(ptr);
    auto obj_ = obj;
    auto met = obj.GetTypeInfo()->GetMethod("display");
    met->Invoke(obj); 

    auto obj2 = Object(std::ref(ref));
    cout << typeid(ref).name() << endl;
    auto met2 = obj2.GetTypeInfo()->GetMethod("display");
    met2->Invoke(obj2);

    auto constructor = typeof(B).GetConstructor(ARGTYPE(int));
    auto obj3 = constructor->Invoke(9);
    ((B)obj3).display();

    auto met3 = obj3.GetTypeInfo()->GetMethod("display");
    met3->Invoke(obj3);

    RegisterMethod(NORMALMETHOD(int, Sub, ARGS(int, int), NONE));
    auto subinfo = GetMethod("Sub", ARGTYPE(int, int));
    auto ret = subinfo->Invoke(89, 75);
    cout << (int)ret << endl;

    RegisterMethod(NORMALMETHOD(double, mult, ARGS(int, int), STATIC));
    auto multinfo = GetMethod("mult", ARGTYPE(int, int));
    auto ret2 = multinfo->Invoke(89, 75);
    cout << (double)ret2 << endl;    
    return 0;
}