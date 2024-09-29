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
        ));
    }
};

int main()
{
    RegisterEnum(NORMALENUM(Fruits, NONE, ENUMVALUE(Fruits, apple), ENUMVALUE(Fruits, orange), ENUMVALUE(Fruits, banana)));
    auto enumInfo = GetEnumType("Fruits");
    cout << (Fruits)enumInfo->GetEnumValue("orange") << endl;
    cout << (Fruits)enumInfo->GetEnumValue("apple") << endl;

    auto typeInfoTest = typeof(Test);
    auto enumInfoTest = typeInfoTest.GetEnum("m_fruit");
    auto enumInfoTest2 = typeInfoTest.GetEnum("m_fruit2");
    Object test{Test()};
    cout << (Fruits)(enumInfoTest->InvokeGet(test)) << endl;
    enumInfoTest->InvokeSet(test, banana);
    cout << (Fruits)(enumInfoTest->InvokeGet(test)) << " " << banana << endl;
    
    cout << (Fruits)(enumInfoTest2->InvokeGet(test)) << endl;
    enumInfoTest2->InvokeSet(test, banana);
    cout << (Fruits)(enumInfoTest2->InvokeGet(test)) << " " << banana << endl;
    return 0;
}