#include <gtest/gtest.h>
#include <Reflection.h>
#include <bits/stdc++.h>

using namespace std;
using namespace Reflection;

//不支持参数含有右值引用的函数的反射
int add(int x, int y){return x + y;}
int add(int *x, int *y){return *x + *y;}
void add_ref(int& x, int y){x = x + y;}

TEST(MethodTest, NormalMethod)
{
    RegisterMethod(NORMALMETHOD(int, add, ARGS(int, int), NONE));
    RegisterMethod(NORMALMETHOD(int, add, ARGS(int*, int*), NONE));
    RegisterMethod(NORMALMETHOD(void, add_ref, ARGS(std::reference_wrapper<int>, int), NONE));
    auto add1 = GetMethod("add", ARGTYPE(int, int));
    auto add2 = GetMethod("add", ARGTYPE(int*, int*));
    auto add_ref1 = GetMethod("add_ref", ARGTYPE(std::reference_wrapper<int>, int));
    int x = 6, y = 90;
    int& xx = x;
    int yy = 90;
    EXPECT_EQ((int)add1->Invoke(34, 45), 79);
    EXPECT_EQ((int)add2->Invoke(&x, &y), 96);
    std::ref(xx).get() += 1;
    EXPECT_EQ(xx, 7);
    add_ref1->Invoke(std::ref(xx), yy);
    EXPECT_EQ(xx, 97);
}