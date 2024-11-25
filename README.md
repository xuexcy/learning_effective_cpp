# [Effective C++](https://github.com/xuexcy/public_attachments/blob/main/books/Effective%20C%2B%2B%E4%B8%AD%E6%96%87%E7%89%88%EF%BC%88%E7%AC%AC%E4%B8%89%E7%89%88%EF%BC%89.pdf)
1. 视 C++ 为一个语言联邦(View C++ as a federation of languages)
    > C++ 高效变成守则视情况而变化，取决于你使用 C++ 的哪一部分
    - C
    - Object-Oriented C++
    - Template C++
    - STL

2. 尽量以 const, enum, inline 替换 #define(Prefer consts, enums, and inlines to #defines)
    > 对于单纯常量，最好 const 对象或 enums 替换 #defines  
    对于形似函数的宏(macros)，最好改用 inline 函数替换 #defines
    - 宏在预处理阶段，如果在编译时出错，报错信息可能根本看不懂。你看到的报错信息是宏预处理后展开的代码的报错信息，假设这个宏不是你定义的，可能会一脸懵
    - 宏展开后的行为可能不符合预期
1. 尽可能使用 const(Use const whenever possible)
    - 尽量加上 const，让编译器进行一些辅助检查
    - 函数返回值是用户自定义类型，加上 const，避免返回值可以被赋值
    - 使用 mutable 在 const 成员函数中修改成员变量
    - non-const 成员函数调用 const 成员函数，避免函数重复: `return const_cast<XXX>(static_cast(ThisClass&)(&this).const_function());`
2. 确定对象被使用前已经被初始化(Make sure that objects are initialized before they're used)
    - 确保内置类型初始化，使用初值列初始化而不是构造函数内
    - 类成员的初始化顺序和声明顺序一致
    - 使用 local static 来解决 non-local static 初始化顺序不确定的问题
    - 多线程下使用 non-const static， 在程序启动时手动初始化(call reference-returning 函数)，避免 race condition
3. 了解 C++ 默默编写并调用了哪些函数(Know what functions C++ silently writes and calls)
    - 编译器会创建默认构造函数(default constructor)、拷贝构造函数(copy constructor)、移动构造函数(move constructor)、拷贝赋值操作符(copy assignment operator，即 `operator=`)、移动赋值操作符(move assignment operator)、析构函数(destructor)。当然，前提是这些函数被调用，可以从 clang++ 为 src/05/main.cc 生成 ast(Abstract Syntax Trees) 看出这一点(执行 `cd src/05 && sh gen_main_ast.sh`)
    - 编译器生成的 copy constructor，对自定义类型使用类型自带的 copy constructor，对内置类型直接按 bits 拷贝
    - 编译器生成的 copy assignment operator 与上述的 copy constructor 差不多，不过编译器在某些情况下会拒绝生成 copy assignment operator，比如类成员含有 reference、const 或者基类的 copy assignment operator 为 private
4. 若不想使用编译器自动生成的函数，就该明确拒绝(Explicitly disallow the use of compiler-generated functions you do not want)
    - 将不想要编译器自动生成的函数声明为 private 来阻止编译器生成(编译器生成的是 public)
    - 对于 friend 或类中的其他函数，依然可以使用这些 private 函数，只声明并使用的话，错误会发生在链接时；将这些 private 函数放到一个 class base 中并继承，这样 friend 和类中的其他函数在使用时，也会发生编译错误，而不是让错误延后到链接时发生
5. 为多态基类声明 virtual 析构函数(Declare destructor virtual in polymorphic base classes)
    - 任何 class 只要带有 virtual 函数都应该有一个 virtual 析构函数，否则可能会出现“局部销毁”(delete 基类指针时只调用了基类的析构函数、销毁基类的成员变量)
    - 如果 class 不含 virtual 函数，通常表示它可能不想被当成 base class，此时令其析构函数为 virtual 是个馊主意(也就是 virtual destructor 需要和其他 virtual 函数一起配合使用)，至少这会使 class 实例大小增加一个 vptr 大小。也就是说如果要使用多态时，不要继承一个不带 virtual 析构函数的类
    - 析构函数必须有定义(即使是纯虚函数)，因为 derived class 的析构函数会调用 base class 的析构函数
    - 并不是所有 base classes 的设计目的都是为了多态
6. 别让异常逃离析构函数(Prevent exceptions from leaving destructors)
    - 析构时发生异常可能导致资源泄露，异常 必须来自析构函数之外的其他函数，如果析构函数调用的函数可能发生异常，析构函数必须吞下异常或终止程序
    - 给用户提供处理异常的机会，如果用户忽略它，我们需要替他们处理
