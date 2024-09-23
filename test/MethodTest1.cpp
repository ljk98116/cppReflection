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
    virtual ~B(){}
    void display() override
    {
        cout << "this is class B" << endl;
    }
    static auto Register()
    {
        return Type<B>(VirtualType::VIRTUAL)
        .AddBaseClass(BASE(A, PUBLIC, VIRTUAL, NONVIRTUAL))
        .AddMethod(NORMALMEMBERMETHOD(display, &B::display, PUBLIC, NONE, NONVIRTUAL));
    }
};

int main()
{
    B b;
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
    return 0;
}