#include <bits/stdc++.h>
#include <Reflection.h>

using namespace std;
using namespace Reflection;

class Person
{
public:
    int x;
    double y;
    std::string z;
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
    static auto Register()
    {
        return Type<Person>()
        .AddProperty(PROPERTY(Person, x, X, PUBLIC, NONE))
        .AddProperty(PROPERTY(Person, y, Y, PUBLIC, NONE))
        .AddProperty(PROPERTYREADONLY(Person, z, Z, PUBLIC, NONE))
        .AddField(FIELD(Person, x, PUBLIC, NONE))
        .AddField(FIELD(Person, y, PUBLIC, NONE));
    }
};

int main()
{
    auto typeInfo_person = typeof(Person);
    auto propX = typeInfo_person.GetProperty("X");
    auto propY = typeInfo_person.GetProperty("Y");
    cout << propX->Name() << " " << AccessType2String(propX->GetAccess()) << endl;
    cout << propY->Name() << " " << AccessType2String(propY->GetAccess()) << endl;

    Person test(12, 45.6, "I am bigger.");
    Object obj(test);
    cout << obj.GetData(typeInfo_person).x << " " << obj.GetData(typeInfo_person).y << endl;
    propX->InvokeSet(obj, 56);
    propY->InvokeSet(obj, 67.9);
    
    cout << obj.GetData(typeInfo_person).x << " " << obj.GetData(typeInfo_person).y << endl;
    cout << obj.GetData(typeInfo_person).z << endl;
    return 0;
}