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
4. 确定对象被使用前已经被初始化(Make sure that objects are initialized before they're used)
    - 确保内置类型初始化，使用初值列初始化而不是构造函数内
    - 类成员的初始化顺序和声明顺序一致
    - 使用 local static 来解决 non-local static 初始化顺序不确定的问题
    - 多线程下使用 non-const static， 在程序启动时手动初始化(call reference-returning 函数)，避免 race condition
5. 了解 C++ 默默编写并调用了哪些函数(Know what functions C++ silently writes and calls)
    - 编译器会创建默认构造函数(default constructor)、拷贝构造函数(copy constructor)、移动构造函数(move constructor)、拷贝赋值操作符(copy assignment operator，即 `operator=`)、移动赋值操作符(move assignment operator)、析构函数(destructor)。当然，前提是这些函数被调用，可以从 clang++ 为 src/05/main.cc 生成 ast(Abstract Syntax Trees) 看出这一点(执行 `cd src/05 && sh gen_main_ast.sh`)
    - 编译器生成的 copy constructor，对自定义类型使用类型自带的 copy constructor，对内置类型直接按 bits 拷贝
    - 编译器生成的 copy assignment operator 与上述的 copy constructor 差不多，不过编译器在某些情况下会拒绝生成 copy assignment operator，比如类成员含有 reference、const 或者基类的 copy assignment operator 为 private
