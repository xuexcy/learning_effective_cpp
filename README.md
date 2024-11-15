# [Effective C++](https://github.com/xuexcy/public_attachments/blob/main/books/Effective%20C%2B%2B%E4%B8%AD%E6%96%87%E7%89%88%EF%BC%88%E7%AC%AC%E4%B8%89%E7%89%88%EF%BC%89.pdf)
1. 视 C++ 为一个语言联邦(View C++ as a federation of languages)
    - C
    - Object-Oriented C++
    - Template C++
    - STL
2. 尽量以 const, enum, inline 替换 #define(Prefer consts, enums, and inlines to #defines)
    - 宏在预处理阶段，如果在编译时出错，报错信息可能根本看不懂。你看到的报错信息是宏预处理后展开的代码的报错信息，假设这个宏不是你定义的，可能会一脸懵
    - 宏展开后的行为可能不符合预期
3. 尽可能使用 const(Use const whenever possible)
    - 尽量加上 const，让编译器进行一些辅助检查
    - 函数返回值是用户自定义类型，加上 const，避免返回值可以被赋值
    - 使用 mutable 在 const 成员函数中修改成员变量
    - non-const 成员函数调用 const 成员函数，避免函数重复: `return const_cast<XXX>(static_cast(ThisClass&)(&this).const_function());`
