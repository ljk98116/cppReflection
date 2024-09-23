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
        .AddMethod(NORMALMEMBERMETHOD(display, &A::display, PUBLIC, NONE, VIRTUAL));
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
    static auto Register()
    {
        return Type<B>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(A, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddConstructor(CONSTRUCTOR(PUBLIC, B, int))
        .AddMethod(NORMALMEMBERMETHOD(display, &B::display, PUBLIC, NONE, NONVIRTUAL));
    }
private:
    int m_x;
};

int main()
{
    B b(9);
    A* ptr = &b;
    A& ref = b;
    ptr->display();
    REGISTERRTTICLASS(B);
    auto obj = Object(ptr);
    auto met = obj.GetTypeInfo()->GetMethod("display");
    met->Invoke(obj);

    auto obj2 = Object(std::ref(ref));
    cout << typeid(ref).name() << endl;
    auto met2 = obj2.GetTypeInfo()->GetMethod("display");
    met2->Invoke(obj2);

    auto constructor = typeof(B).GetConstructor(ARGTYPE(int));
    auto obj3 = constructor->Invoke(9);
    ((B)obj3).display();
    //auto met3 = obj3.GetTypeInfo()->GetMethod("display");
    //met3->Invoke(obj3);
    return 0;
}