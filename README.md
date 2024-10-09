# cppReflection

## 1.Property类型

类似于C#，我们可以可以在类中定义自己的GET/SET方法，同时可以通过宏定义给出对应属性是正常[外部提供GET、SET方法]、只读[外部仅提供GET方法]、默认[使用默认的GET/SET方法]、默认只读[使用默认的GET方法]，支持多继承和虚继承

## 2.Type类型
Type信息使用类模板存储,可以使用typeof宏获取对应类型的信息，返回模板类Type\<xxx\>
```
auto xxxinfo = typeof(xxx);
```

## 3.Object类型
需要利用Type信息进行内存管理,支持任意类型的显式转化和隐式转化
```
Object obj(std::string("OK"));
std::cout << (std::string)obj << std::endl;
```

## 4.Method
支持类成员函数、类构造函数、类析构函数、类静态成员函数、普通函数Invoke，支持函数重载、动态多态、默认参数、传递引用。用法示例见examples/MethodExample.cpp

## 5.Field类型
类似于Property，但仅提供默认的InvokeGet/InvokeSet


