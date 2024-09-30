#include <bits/stdc++.h>
#include <Reflection.h>

using namespace std;
using namespace Reflection;

enum Fruits
{
    apple, orange=6, banana
};

class Test
{
public:
    Fruits m_fruit = orange;
    Fruits m_fruit2 = apple;
    static Fruits m_staticfruit;
    static auto Register()
    {
        return Type<Test>().AddEnum(
            NORMALMEMBERENUM(m_fruit, Test, m_fruit, PUBLIC, 
                ENUMVALUE(Fruits, apple), 
                ENUMVALUE(Fruits, orange), 
                ENUMVALUE(Fruits, banana)
        ))
        .AddEnum(
            NORMALMEMBERENUM(m_fruit2, Test, m_fruit2, PUBLIC, 
                ENUMVALUE(Fruits, apple), 
                ENUMVALUE(Fruits, orange), 
                ENUMVALUE(Fruits, banana)  
        ))
        .AddEnum(
            STATICMEMBERENUM(m_staticfruit, Test, m_staticfruit, PUBLIC, 
                ENUMVALUE(Fruits, apple), 
                ENUMVALUE(Fruits, orange), 
                ENUMVALUE(Fruits, banana)             
        ));
    }
};

Fruits Test::m_staticfruit = banana;

Fruits fruit = apple;
int main()
{
    RegisterEnum(NORMALENUM(Fruits, NONE, ENUMVALUE(Fruits, apple), ENUMVALUE(Fruits, orange), ENUMVALUE(Fruits, banana)));
    auto enumInfo = GetEnumType("Fruits");
    cout << (Fruits)enumInfo->GetEnumValue("orange") << endl;
    cout << (Fruits)enumInfo->GetEnumValue("apple") << endl;
    Object obj(fruit);
    cout << (Fruits)(enumInfo->InvokeGet(obj)) << endl;
    enumInfo->InvokeSet(obj, orange);
    cout << (Fruits)(enumInfo->InvokeGet(obj)) << endl;

    auto typeInfoTest = typeof(Test);
    auto enumInfoTest = typeInfoTest.GetEnum("m_fruit");
    auto enumInfoTest2 = typeInfoTest.GetEnum("m_fruit2");
    auto enumInfoTest3 = typeInfoTest.GetEnum("m_staticfruit");
    Object test{Test()};
    cout << (Fruits)(enumInfoTest->InvokeGet(test)) << endl;
    enumInfoTest->InvokeSet(test, banana);
    cout << (Fruits)(enumInfoTest->InvokeGet(test)) << " " << banana << endl;
    
    cout << (Fruits)(enumInfoTest2->InvokeGet(test)) << endl;
    enumInfoTest2->InvokeSet(test, banana);
    cout << (Fruits)(enumInfoTest2->InvokeGet(test)) << " " << banana << endl;

    cout << (Fruits)(enumInfoTest3->InvokeGet()) << endl;
    enumInfoTest3->InvokeSet(apple);
    cout << (Fruits)(enumInfoTest3->InvokeGet()) << " " << apple << endl;
    return 0;
}