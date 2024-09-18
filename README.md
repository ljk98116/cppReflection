# cppReflection

## 1.属性反射

类似于C#，我们可以可以在类中定义自己的GET/SET方法，同时可以通过宏定义给出对应属性是正常[外部提供GET、SET方法]、只读[外部仅提供GET方法]、默认[使用默认的GET/SET方法]、默认只读[使用默认的GET方法]

## 2.Type类型
Type信息使用类模板存储

## 3.Object类型
需要利用Type信息进行内存管理

属性反射使用典型例子如下：
```
#include <bits/stdc++.h>
#include <Reflection.h>

using namespace std;
using namespace Reflection;

class Person
{
private:
    int x;
    double y;
    std::string z;
    static int ok;
public:
    Person(){}
    Person(int x, double y, std::string z):x(x),y(y),z(z){}
    SET(X, int)
    {
        x=value;
    }
    GET(X, int)
    {
        return x;
    }
    SET(Y, double)
    {
        x += 6;
        y=value;
    }
    GET(Y, double)
    {
        return y;
    }
    GET(Z, std::string)
    {
        return z;
    }
    STATIC_GET(OK, int)
    {
        return Person::ok;
    }
    STATIC_SET(OK, int)
    {
        Person::ok = value;
    }
    static auto Register()
    {
        return Type<Person>()
        .AddProperty(PROPERTY(Person, x, X, PUBLIC, NONE))
        .AddProperty(PROPERTY(Person, y, Y, PUBLIC, NONE))
        .AddProperty(PROPERTYREADONLY(Person, z, Z, PRIVATE, NONE))
        .AddProperty(STATIC_PROPERTY(Person, ok, OK, PUBLIC));
    }
};

int Person::ok = 45;

int main()
{
    auto typeInfo_person = typeof(Person);
    auto propX = typeInfo_person.GetProperty("X");
    auto propY = typeInfo_person.GetProperty("Y");
    auto propZ = typeInfo_person.GetProperty("Z");
    auto propOK = typeInfo_person.GetProperty("OK");

    cout << propX->Name() << " " << AccessType2String(propX->GetAccess()) << endl;
    cout << propY->Name() << " " << AccessType2String(propY->GetAccess()) << endl;
    cout << propZ->Name() << " " << AccessType2String(propZ->GetAccess()) << endl;
    cout << propOK->Name() << " " << StaticType2String(propOK->GetStaticType()) << endl;

    Person test(12, 45.6, "I am bigger.");
    
    Object obj(test);
    cout << obj.GetData(typeInfo_person).getX() << " " << obj.GetData(typeInfo_person).getY() << endl;
    propX->InvokeSet(obj, 56);
    propY->InvokeSet(obj, 67.9);
    propOK->InvokeSet(90);

    cout << obj.GetData(typeInfo_person).getX() << " " << obj.GetData(typeInfo_person).getY() << endl;
    cout << obj.GetData(typeInfo_person).getZ() << endl;
    cout << Person::getOK() << endl;

    //runtime error expected
    //auto zobj = propZ->InvokeGet(obj);
    //propZ->InvokeSet(obj, "error occured");

    return 0;
}
```
目前属性反射可以支持虚继承，典型例子(test/PropertyTestOOP.cpp)

