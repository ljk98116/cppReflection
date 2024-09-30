#include <gtest/gtest.h>
#include <Reflection.h>
#include <bits/stdc++.h>

using namespace std;
using namespace Reflection;

class TestDefaultGetSet
{
public:
    static auto Register()
    {
        return Type<TestDefaultGetSet>()
        .AddProperty(PROPERTYDEFAULT(TestDefaultGetSet, m_x, X, PUBLIC, NONE))
        .AddProperty(PROPERTYDEFAULT(TestDefaultGetSet, m_y, Y, PUBLIC, NONE))
        .AddProperty(PROPERTYDEFAULT(TestDefaultGetSet, m_t, T, PUBLIC, NONE))
        .AddProperty(PROPERTYDEFAULT(TestDefaultGetSet, m_v, V, PUBLIC, NONE))
        .AddProperty(STATIC_PROPERTYDEFAULT(TestDefaultGetSet, m_u, U, PUBLIC));
    }
public:
    int m_x;
    char m_y;
    static int m_u;
private:
    string m_t;
    vector<int> m_v;
};

int TestDefaultGetSet::m_u = 8;

TEST(PropertyTest, DefaultGetSet)
{
    auto TestDefaultGetSetInfo = typeof(TestDefaultGetSet);
    auto propInfo_X = TestDefaultGetSetInfo.GetProperty("X");
    auto propInfo_Y = TestDefaultGetSetInfo.GetProperty("Y");
    auto propInfo_T = TestDefaultGetSetInfo.GetProperty("T");
    auto propInfo_V = TestDefaultGetSetInfo.GetProperty("V");
    auto propInfo_U = TestDefaultGetSetInfo.GetProperty("U");

    Object obj(TestDefaultGetSet{});
    propInfo_X->InvokeSet(obj, 7);
    EXPECT_EQ((int)(propInfo_X->InvokeGet(obj)), 7);
    propInfo_X->InvokeSet(obj, 800);
    EXPECT_EQ((int)(propInfo_X->InvokeGet(obj)), 800);

    propInfo_Y->InvokeSet(obj, '7');
    EXPECT_EQ((char)(propInfo_Y->InvokeGet(obj)), '7');
    propInfo_Y->InvokeSet(obj, 'P');
    EXPECT_EQ((char)(propInfo_Y->InvokeGet(obj)), 'P');   

    propInfo_T->InvokeSet(obj, string("ui"));
    EXPECT_EQ((string)(propInfo_T->InvokeGet(obj)), "ui");
    propInfo_T->InvokeSet(obj, string("OK"));
    EXPECT_EQ((string)(propInfo_T->InvokeGet(obj)), "OK");

    auto vec1 = vector<int>{2, 3, 4};
    auto vec2 = vector<int>{2, 3, 5};
    propInfo_V->InvokeSet(obj, vec1);
    EXPECT_EQ(propInfo_V->InvokeGet(obj).GetData<vector<int> >(), vec1);
    propInfo_V->InvokeSet(obj, vec2);
    EXPECT_EQ(propInfo_V->InvokeGet(obj).GetData<vector<int> >(), vec2); 

    auto t2 = TestDefaultGetSet();
    Object obj2(std::move(t2));
    propInfo_X->InvokeSet(obj2, 7);
    EXPECT_EQ(((TestDefaultGetSet)obj2).m_x, 7);
    propInfo_X->InvokeSet(obj2, 800);
    EXPECT_EQ(((TestDefaultGetSet)obj2).m_x, 800);

    propInfo_Y->InvokeSet(obj2, '7');
    EXPECT_EQ(((TestDefaultGetSet)obj2).m_y, '7');
    propInfo_Y->InvokeSet(obj2, 'P');
    EXPECT_EQ(((TestDefaultGetSet)obj2).m_y, 'P');

    propInfo_U->InvokeSet(89);
    EXPECT_EQ(TestDefaultGetSet::m_u, 89);
}

class TestGetSet
{
public:
    static auto Register()
    {
        return Type<TestGetSet>()
        .AddProperty(PROPERTY(TestGetSet, m_x, X, PUBLIC, NONE))
        .AddProperty(PROPERTYREADONLY(TestGetSet, m_y, Y, PUBLIC, NONE))
        .AddProperty(PROPERTYDEFAULT(TestGetSet, m_t, T, PROTECT, NONE))
        .AddProperty(PROPERTYDEFAULT(TestGetSet, m_v, V, PRIVATE, NONE));
    }
public:
    int m_x;
    char m_y = 'p';
    SET(X, int)
    {
        m_x = value * 2;
    }
    GET(X, int)
    {
        return m_x / 2;
    }
    GET(Y, char)
    {
        return m_y;
    }
private:
    string m_t;
    vector<int> m_v;
};

TEST(PropertyTest, NormalGetSet_with_access)
{
    auto TestGetSetInfo = typeof(TestGetSet);
    auto propInfo_X = TestGetSetInfo.GetProperty("X");
    auto propInfo_Y = TestGetSetInfo.GetProperty("Y");
    auto propInfo_T = TestGetSetInfo.GetProperty("T");
    auto propInfo_V = TestGetSetInfo.GetProperty("V");

    auto t = TestGetSet();
    Object obj(std::move(t));
    propInfo_X->InvokeSet(obj, 6);
    EXPECT_EQ((int)propInfo_X->InvokeGet(obj), 6);

    EXPECT_EQ((char)propInfo_Y->InvokeGet(obj), 'p');
    EXPECT_THROW(propInfo_Y->InvokeSet(obj, 'o'), std::runtime_error);

    EXPECT_THROW(propInfo_T->InvokeSet(obj, string("ui")), std::runtime_error);
    EXPECT_THROW(propInfo_T->InvokeGet(obj), std::runtime_error);
}

class TestInheritC1
{
public:
    static auto Register()
    {
        return Type<TestInheritC1>().AddProperty(PROPERTYDEFAULT(TestInheritC1, m_x, X, PUBLIC, NONE));
    }
    int m_x;
};

class TestInheritC2
{
public:
    static auto Register()
    {
        return Type<TestInheritC2>().AddProperty(PROPERTYDEFAULT(TestInheritC2, m_y, Y, PUBLIC, NONE));
    }
    string m_y;    
};

class TestInheritC3 : public TestInheritC1, public TestInheritC2
{
public:
    virtual ~TestInheritC3(){}
    static auto Register()
    {
        return Type<TestInheritC3>(VIRTUAL)
        .AddBaseClass(BASE(TestInheritC1, PUBLIC, NONVIRTUAL, NONVIRTUAL))
        .AddBaseClass(BASE(TestInheritC2, PUBLIC, NONVIRTUAL, NONVIRTUAL));
    }
};

TEST(PropertyTest, GetSet_in_inherit)
{
    auto TestInheritC3Info = typeof(TestInheritC3);
    auto propInfo_X = TestInheritC3Info.GetProperty("X");
    auto propInfo_Y = TestInheritC3Info.GetProperty("Y");
    cout << propInfo_Y->Name() << " " << propInfo_Y->GetClassName() << endl;

    Object obj2(TestInheritC3{});
    propInfo_X->InvokeSet(obj2, 7);
    EXPECT_EQ(((TestInheritC3)obj2).m_x, 7);
    propInfo_X->InvokeSet(obj2, 800);
    EXPECT_EQ(((TestInheritC3)obj2).m_x, 800);

    propInfo_Y->InvokeSet(obj2, string("ui"));
    EXPECT_EQ(((TestInheritC3)obj2).m_y, "ui");
    propInfo_Y->InvokeSet(obj2, string("OK"));
    EXPECT_EQ(((TestInheritC3)obj2).m_y, "OK");
}