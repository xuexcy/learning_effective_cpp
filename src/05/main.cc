/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/21 15:32:24
# Desc    :   条款 05: 了解 C++ 默默编写并调用了哪些函数(Know what functions C++ silently writes and calls)
########################################################################
*/

// part-1
// 编译器默认声明 copy 构造函数、copy assignment 操作符、析构函数
// 如果自己没有声明任何构造函数，编译器会声明 default 构造函数
// 当然，只有这些函数被调用时，才会真正的被编译器创造出来，且都是 public inline

// 下面的 ast 是 clang++ 生成的语法树的部分展示，执行 ./gen_main_ast.sh 即可
// 可以看到编译器并不会生成所有的默认函数，只有使用的才会生成
// 1. user_defined::Empty 只使用了默认构造函数和拷贝构造函数(编译器还生成了移动构造函数)
// 2. user_defined_2::Empty 只使用了默认构造函数和拷贝赋值函数(编译器还生成了拷贝构造、移动构造、移动赋值函数)
namespace user_defined {
class Empty {

    // 编译器生成默认构造函数
    // ast: -CXXConstructorDecl implicit used constexpr Empty 'void () noexcept' inline default trivial
    // 编译器生成拷贝构造函数
    // ast: -CXXConstructorDecl implicit used constexpr Empty 'void (const Empty &) noexcept' inline default trivial
    // 编译器生成移动构造函数
    // ast: -CXXConstructorDecl implicit constexpr Empty 'void (Empty &&)' inline default trivial noexcept-unevaluated

};  // class Empty
}  // namespace user_defined

namespace user_defined_2 {
class Empty {

    // 编译器生成默认构造函数
    // ast: -CXXConstructorDecl implicit used constexpr Empty 'void () noexcept' inline default trivial
    // 编译器生成拷贝构造函数
    // ast: -CXXConstructorDecl implicit constexpr Empty 'void (const Empty &)' inline default trivial noexcept-unevaluated
    // 编译器生成移动构造函数
    // ast: -CXXConstructorDecl implicit constexpr Empty 'void (Empty &&)' inline default trivial noexcept-unevaluated
    // 编译器生成拷贝赋值函数
    // ast: -CXXConstructorDecl implicit used constexpr operator= 'Empty &(const Empty &) noexcept' inline default trivial
    // 编译器生成移动赋值函数
    // ast: -CXXConstructorDecl implicit constexpr operator= 'Empty &(Empty &&)' inline default trivial noexcept-unevaluated

};  // class Empty
}  // namespace user_defined_2

namespace compiler_generate {
class Empty {
public:

    // ast: -CXXConstructorDecl constexpr Empty 'void ()' default trivial noexcept-unevaluated 0x14f8cb4a8 implicit-inline
    Empty() = default;

    // ast: -CXXConstructorDecl constexpr Empty 'void (const Empty &)' default trivial noexcept-unevaluated 0x14f8cb748 implicit-inline
    Empty(const Empty& rhs) = default;
    // ast: -CXXConstructorDecl constexpr operator= 'Empty &(const Empty &)' default trivial noexcept-unevaluated 0x14f8cb9e0 implicit-inline
    Empty& operator=(const Empty& rhs) = default;
    // ast: --CXXDestructorDecl constexpr ~Empty 'void ()' default trivial noexcept-unevaluated 0x14f8cbaf8 implicit-inline
    ~Empty() = default;

};  // class Empty
}  // namespace compiler_generate

void run_empty() {
    {
        // ast: -CXXConstructExpr 'user_defined::Empty' 'void () noexcept'
        user_defined::Empty e1;
        // ast: -CXXConstructExpr 'user_defined::Empty' 'void (const Empty &) noexcept'
        user_defined::Empty e2(e1);  // copy constructor
    }
    {
        // ast: -CXXConstructExpr 'user_defined_2::Empty' 'void () noexcept'
        user_defined_2::Empty e1;
        // ast: -CXXConstructExpr 'user_defined_2::Empty' 'void () noexcept'
        user_defined_2::Empty e2;
        // ast: -CXXOperatorCallExpr
        // ast: 'operator=' 'Empty &(const Empty &) noexcept'
        e2 = e1;  // copy assignment
    }
}

int main() {
    run_empty();
    return 0;
}

