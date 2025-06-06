# [Effective C++](https://github.com/xuexcy/public_attachments/blob/main/books/Effective%20C%2B%2B%E4%B8%AD%E6%96%87%E7%89%88%EF%BC%88%E7%AC%AC%E4%B8%89%E7%89%88%EF%BC%89.pdf)
# 1. 让自己习惯 C++(Accustoming Yourself to C++)
1. 视 C++ 为一个语言联邦(View C++ as a federation of languages)
    > C++ 高效变成守则视情况而变化，取决于你使用 C++ 的哪一部分
    - C
    - Object-Oriented C++
    - Template C++
    - STL
2. 尽量以 const, enum, inline 替换 #define(Prefer consts, enums, and inlines to #defines)
    > 对于单纯常量，最好 const 对象或 enums 替换 #defines
    >
    > 对于形似函数的宏(macros)，最好改用 inline 函数替换 #defines
    - 宏在预处理阶段，如果在编译时出错，报错信息可能根本看不懂。你看到的报错信息是宏预处理后展开的代码的报错信息，假设这个宏不是你定义的，可能会一脸懵
    - 宏展开后的行为可能不符合预期
3. 尽可能使用 const(Use const whenever possible)
    > 将某些东西声明为 const 可帮助编译器侦测出错误用户。const 可被施加于任何作用域内的对象、函数参数、函数返回类型、成员函数本体
    >
    > 编译器强制实施 bitwise constness，但你编写程序时应该使用“概念上的常量性”(conceptual constness)
    >
    > 当 const 和 non-const 成员函数有着实质等价的实现时，令 non-const 版本调用 const 版本可避免代码重复
    - 尽量加上 const，让编译器进行一些辅助检查
    - 函数返回值是用户自定义类型，加上 const，避免返回值可以被赋值
    - 使用 mutable 在 const 成员函数中修改成员变量
    - non-const 成员函数调用 const 成员函数，避免函数重复: `return const_cast<XXX>(static_cast(ThisClass&)(&this).const_function());`
4. 确定对象被使用前已经被初始化(Make sure that objects are initialized before they're used)
    > 为内置型对象进行手工初始化，因为 C++不保证初始化它们
    >
    > 构造函数最好使用成员初始列(member initialization list)，而不要在构造函数本体内使用赋值操作(assignment)。初始列列出的成员变量，其排列次序应该和他们在 class 中的声明次序相同
    >
    > 为免除“跨编译单元之初始化次序”问题，请以 local static 对象替换 non-local static 对象
    - 确保内置类型初始化，使用初值列初始化而不是构造函数内
    - 类成员的初始化顺序和声明顺序一致
    - 使用 local static 来解决 non-local static 初始化顺序不确定的问题
    - 多线程下使用 non-const static， 在程序启动时手动初始化(call reference-returning 函数)，避免 race condition
# 2. 构造/析构/赋值运算(Constructors, Destructors, and Assignment Operators)
5. 了解 C++ 默默编写并调用了哪些函数(Know what functions C++ silently writes and calls)
    > 编译器可以暗自为 class 创建 _default_ 构造函数、_copy_ 函数、_copy assignment_ 操作符，以及析构函数
    - 编译器会创建默认构造函数(default constructor)、拷贝构造函数(copy constructor)、移动构造函数(move constructor)、拷贝赋值操作符(copy assignment operator，即 `operator=`)、移动赋值操作符(move assignment operator)、析构函数(destructor)。当然，前提是这些函数被调用，可以从 clang++ 为 src/05/main.cc 生成 ast(Abstract Syntax Trees) 看出这一点(执行 `cd src/05 && sh gen_main_ast.sh`)
    - 编译器生成的 copy constructor，对自定义类型使用类型自带的 copy constructor，对内置类型直接按 bits 拷贝
    - 编译器生成的 copy assignment operator 与上述的 copy constructor 差不多，不过编译器在某些情况下会拒绝生成 copy assignment operator，比如类成员含有 reference、const 或者基类的 copy assignment operator 为 private
6. 若不想使用编译器自动生成的函数，就该明确拒绝(Explicitly disallow the use of compiler-generated functions you do not want)
    > 为驳回编译器自动(暗自)提供的机能，可将相应的成员函数声明为 private 并且不予实现。使用像 Uncopyable 这样的 base class 也是一种做法
    - 将不想要编译器自动生成的函数声明为 private 来阻止编译器生成(编译器生成的是 public)
    - 对于 friend 或类中的其他函数，依然可以使用这些 private 函数，只声明并使用的话，错误会发生在链接时；将这些 private 函数放到一个 class base 中并继承，这样 friend 和类中的其他函数在使用时，也会发生编译错误，而不是让错误延后到链接时发生
7. 为多态基类声明 virtual 析构函数(Declare destructor virtual in polymorphic base classes)
    > polymorphic(带多态性质的)base classes 应该声明一个 virtual 析构函数。如果 class 带有任何 virtual 函数，它就应该拥有一个 virtual 析构函数
    >
    > Classes 的设计目的如果不是作为 base classes 使用，或不是为了具备多态性(polymorphically)，就不应该声明 virtual 析构函数
    - 任何 class 只要带有 virtual 函数都应该有一个 virtual 析构函数，否则可能会出现“局部销毁”(delete 基类指针时只调用了基类的析构函数、销毁基类的成员变量)
    - 如果 class 不含 virtual 函数，通常表示它可能不想被当成 base class，此时令其析构函数为 virtual 是个馊主意(也就是 virtual destructor 需要和其他 virtual 函数一起配合使用)，至少这会使 class 实例大小增加一个 vptr 大小。也就是说如果要使用多态时，不要继承一个不带 virtual 析构函数的类
    - 析构函数必须有定义(即使是纯虚函数)，因为 derived class 的析构函数会调用 base class 的析构函数
    - 并不是所有 base classes 的设计目的都是为了多态
8. 别让异常逃离析构函数(Prevent exceptions from leaving destructors)
    > 析构函数绝对不要吐出异常。如果一个被析构函数调用的函数可能抛出异常，析构函数应该捕捉任何异常，然后吞下它们(不传播)或结束程序
    > 如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么 class 应该提供一个普通函数(而非在析构函数中)执行该操作
    - 析构时发生异常可能导致资源泄露，异常 必须来自析构函数之外的其他函数，如果析构函数调用的函数可能发生异常，析构函数必须吞下异常或终止程序
    - 给用户提供处理异常的机会，如果用户忽略它，我们需要替他们处理
9. 绝不在构造和析构过程中调用 virtual 函数(Never call virtual functions during construction or destruction)
    > 在构造和析构期间不要调用 virtual 函数，因为这类调用从不下降至 derived class(比起当前执行构造函数和析构函数的那层)。
    - 在 base class 的构造和析构函数中，实例的类型都是 base class，调用的虚函数也是 base class 中的虚函数，如果函数是纯虚函数且未定义，会导致程序在执行时找不到函数定义而异常退出
    - 对于需要在 base class 的构造函数中使用一些 derived class 中的数据/信息时，可以通过使用 base class 的构造函数参数来传递信息
10. 令 operator= 返回一个 reference to *this(Have assignment operators return a reference to *this)
    > 令赋值(assignment)操作符返回一个 reference to *this
    - 方便连锁赋值，像 `int x, y, z; x = y = z = 10` 一样
11. 在 operator= 中处理“自我赋值”(Handle assignment to self in operator=)
    > 确保当对象自我复制时 operator= 有良好行为。器中技术包括比较“来源对象”和“目标对象”的地址、精心周到的语句顺序、以及 copy-and-swap。
    > 确定任何函数如果操作一个以上的对象，而其中多个对象时同一个对象时，其行为仍然正确
    - `if (this == &rhs) return *this;`
    - copy-and-swap: `ClassName temp(rhs); swap(this, &rhs); return *this;`
12. 复制对象时勿忘其每一个成分(Copy all parts of object)
    > Copying 函数应该确保赋值“对象内的所有成员变量”及“所有 base class 成分”
    > 不要尝试一某个 copying 函数实现另一个 copying 函数。应该将共同技能放进第三个函数中，并有两个 copying 函数共同调用。
    - class 新添成员变量时，必须同时修改 copy 函数
    - 在继承的 copy 函数中调用基类的 copy 函数
        ```cpp
        // copy constructor
        Derived(const Derived& rhs): Base(rhs), a_(rhs.a_), b_(rhs.b_) {}
        // copy assignment operator
        Derived& (const Derived& rhs) {
            Base::operator=(rhs);
            a_ = rhs.a_;
            b_ = rhs.b_;
        }
        ```
    - 不要在 copy assignment operator 中调用 copy constructor，反之也是如此，如果有很多相同的代码，可以提取一个 private void init() 函数出来
# 3. 资源管理(Resource Management)
13. 以对象管理资源(Use objects to manage resources)
    > 为防止资源泄露，请使用 RAII 对象，他们在构造函数中获得资源并在析构函数中释放资源
    - 使用智能指针来管理和释放资源
14. 在资源管理类中小心 copying 行为(Think carefully about copying behavior in resource-managing classes)
    > 复制 RAII 对象必须一并复制它所管理的资源，所以资源的 copying 行为决定 RAII 对象的 copying 行为
    > 普遍而常见的 RAII class copying 行为是：抑制 copying、施行引用技术法(reference counting)。不过其他行为也都可能被实现。
    - 可以实现 RAII likely 封装以管理资源
    - 可以抑制 RAII 的 copying 行为(Uncopyable)或者使用引用计数(可以指定在计数归 0 时的 deleter 行为 )
    - 可以使用 [function poisoning](https://github.com/xuexcy/learning_more_cpp_idioms/blob/main/src/function_poisoning.cc) 来禁用一些不合适的接口
15. 在资源管理类中提供对原始资源的访问(Provide access to raw resources in resource-managing classes)
    > APIs 往往要求访问原始资源（raw resources），所以每一个 RAII class 应该提供一个“取得其所管理之资源”的办法。
    > 对原始资源的访问可经由显示转换或隐式转换。一般而言显示转换比较安全，但隐式转换对客户比较方便。
    - 对自行封装的 RAII 类，提供一个接口让用户获取原始资源，比如智能指针的 `T* get();`
    - 显示转换：`Data get() const { return d_; }`
    - 隐式转换：`operator Data() const { return d_; }`
16. 成对使用 new 和 delete 时要采取相同形式(Use the same form in corresponding uses of new and delete)
    > 如果你在 new 表达式中使用 []，必须在相应的 delete 表达式中也使用 []。如果你在 new 表达式中不使用 []，一定不要在相应的 delete 表达式中使用 []。
    - new 和 delete 中的 [] 都要配对使用
        ```cpp
        int* a = new int(1);
        delete a;
        int* b = new int[8];
        delete [] b;
        ```
17. 以独立语句将 newed 对象置入智能指针(Store newed objects in smart pointers in standalone statements)
    > 以独立语句将 newed 对象存储与（置入）智能指针内。如果不这样做，一旦异常被抛出，有可能导致难以察觉的资源泄露。
    - 使用 `std::make_shared`
        ```cpp
        // function_throw_exception 可能发生在 new Widget 和 std::shared_ptr<Widget>(widget_ptr) 之间
        // 导致抛出异常后无法通过 std::shared_ptr 回收 new Widget 申请的资源
        // no:
        process_widget(std::shared_ptr<Widget>(new Widget), function_throw_exception());
        // yes:
        process_widget(std::make_shared<Widget>(), function_throw_exception());
        ```
# 4. 设计与声明(Designs and Declarations)
18. 让接口容易被正确使用，不易被误用(Make interface easy to use correctly and hard to use incorrectly)
    > 好的接口很容易被正确使用，不容易被误用。你应该在你的所有借口中努力达成这些性质。
    > “促使正确使用”的办法包括接口的一致性，以及与内置类型的行为兼容。
    > “阻止误用”的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除客户的资源管理责任
    > std::shared_ptr 支持定制型删除器(custom deleter)。这可防范 DLL 问题，可悲用来自动解除互斥锁(mutexes; 见条款 14)等等。
    - `Date (int day, int month, int year);` -> `Date (Day d, Month m, Year y);`
19. 设计 class 犹如设计 type(Treat class design as type design)
20. 宁以 pass-by-reference-to-const 替换 pass-by-value(Prefer pass-by-reference-to-const to pass-by-value)
    > 尽量以 pass-by-reference-to-const 替换 pass-by-value。前者通常比较搞笑，并可避免切割问题(slicing problem)。
    > 以上规则并不适用于内置类型，以及 STL 的迭代器和函数对象。对他们而言，pass-by-value 往往比较适当。
    - pass-by-value 往往造成不必要的构造和析构，以及造成切割问题(将继承类中的信息切除)
21. 必须返回对象时，别妄想返回其 reference(Don't try to return a reference when you must return an object)
    > 绝不要返回 pointer 或 reference 指向一个 local stack 对象，或返回 reference 指向一个 heap-allocated 对象，或返回 pointer 或 reference 指向一个 local static 对象而有可能同时需要多个这样的对象。条款 4 已经为“在单线程环境中合理返回 reference 指向一个 local static 对象”提供了一份设计实例。
22. 将成员变量声明为 private(Declare data members private)
    > 切记将成员变量声明为 private。这可赋予客户访问数据的一致性、可细微划分访问控制、允诺约束条件获得保证，并提供 class 作者以充分的实现弹性。
    > protected 并不比 public 更具封装性
    - 使用的 public 成员变量，如果类的声明逻辑发生修改，可能导致较大的代码改动。比如从 `class SpeedDataCollection` 中获取速度平均值，如果使用 public 成员变量进行计算，计算方式可能会改变(比如 class 从存储所有平均值改为每次存储时叠加速度和计数)，此时通过 `double average() const;` 获取平均值并不需要修改用户代码
23. 宁以 non-member、non-friend 替换 member 函数(Prefer non-member non-friend functions to member functions)
    > 宁可拿 non-member non-friend 函数替换 member 函数。这样做可以增加封装性、包裹弹性（packing flexibility）和机能扩充性。
    - non-mem non-friend 函数不会访问 class 的私有成员，方便迭代 class
    - 将工具/便利函数放到和 class 相同的 namespace，并分类置于不同的 header.h 中，方便用户使用，亦可减少编译依赖
    - 用户也可以封装自己动工具函数，并放到和 class 相同的 namespace 中以便使用
24. 若所有参数皆需类型，请为此采用 non-member 函数(Declare non-member functions when type conversion should apply to all parameters)
    > 如果你需要为某个函数的所有参数（包括被 this 指针所指的那个隐喻参数）进行类型转换，那么这个函数必须是个 non-member。
    - 对于 binary operator，如果 lhs 和 rhs 都需要隐式转换，那么可以将 operator 声明为 non-member 函数
        ```cpp
        class Rational { public: Rational(int n, int d); };
        Rational a(1, 2);
        {
            // class member function
            const Rational Rational::Rational(const Rational& rhs) const;
            // no
            Result res = 2 * a;
        }
        {
            // non-member function
            const Rational operator*(const Rational& lhs, const Rational& rhs);
            // yes
            Result res = 2 * a;
        }
        ```
25. 考虑写一个不抛异常的 swap 函数(Consider support for a non-throwing swap)
    > 当 std::swap 对你的类型效率不高时，提供一个 swap 成员函数，并确定这个函数不抛异常。
    > 如果你提供一个 member swap, 也该提供一个 non-member swap 用来调用前者，对于 classes (而非 templates)，也请特化 std::swap。
    > 调用 swap 时应针对 std::swap 使用 using 声明式，然后调用 swap 并且不带任何“命名空间资格修饰”
    > 为“用户定义类型”进行 std templates 全特化时好的，但千万不要尝试在 std 内加入某些对 std 而言全新的东西。
    - 对于 class：pImpl + member swap + 全特化 std::swap
    - 对于 class template：pImpl + member swap + non-member swap
    - 使用 swap 时：`using std::swap; swap(lhs, rhs);`(named-lookup-rules)
# 5. 实现(Implementations)
26. 尽可能延后变量定义式的出现时间(Postpone variable definitions as long as possible)
    > 尽可能延后变量定义式的出现。这样做可增加程序的清晰度并改善程序效率
    - 提前定义的变量如果没有使用，会造成多余的构造与析构
27. 尽量少做转型动作(Minimize casting)
    > 如果可以，尽量便面转型，特别是在注重效率的代码中避免 dynamic_casts。如果有个设计需要转型 动作，试着发展无需转型的替代设计
    > 如果转型是必要的，试着将它隐藏于某个函数背后。客户随后可以调用该函数，而不需将转型放进他们自己的代码内。
    > 宁可使用 C++-style(新式)转型，不要使用旧式转型。前者很容易识别出来，而且也比较有着分门别类的职掌。
    - 通过 cast 得到的 base 和 derived 指针的地址可能不同
    - 在需要调用基类的函数时，不要使用 cast，那样会新建一个临时变量，函数调用是不符合预期的，可以直接使用 `base::function()` 来调用基类函数
        ```cpp
        // no
        virtual void Derived::function() override {
            static_cast<Base>(*this).function();
        }
        // yes
        virtual void Derived::function() override {
            Base::function();
        }
        ```
    - dynamic_cast 有性能问题，在明确知道可以向下 dynamic_cast 的类型时，可以使用 static_cast，因为 static_cast 会更快（但是不安全，所以需要用户明确知道可以 cast）
