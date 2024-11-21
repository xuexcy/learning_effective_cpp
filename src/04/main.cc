/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/20 14:31:17
# Desc    :   条款 04: 确定对象被使用前已经被初始化(Make sure that objects are initialized before they're used)
########################################################################
*/
#include <algorithm>
#include <print>
#include <thread>
#include <vector>

#include "cpp_utils/util.h"

#include "directory.h"

class A {};
class B {
public:
    B(const A& a, int b, int c) {
        // a 在进入构造函数体之前使用 A 的默认构造函数进行初始化，然后再进入下面的赋值
        // 下面是赋值(assignment)而不是初始化(initialization)
        a_ = a;
        b_ = b;
        c_ = c;
    }
private:
    A a_;
    int b_;
    int c_;
};  // class B

// part-1
// C++ 规定，对象的成员变量的初始化动作发生在进入构造函数本体之前
//  如果可以，请总是使用"成员初值列"(member initialization list)
namespace B1 {
class B {
public:
    B(const A& a, int b, int c):
        a_(a), // 初始化，效率比赋值高
        b_(b), c_(c) {
    }
private:
    A a_;
    int b_;
    int c_;
};  // class B
}  // namespace B1

// part2
// 对于内置类型(比如 int b_)，初始化和赋值的成本相同
// 如果 class 有多个构造函数， 且有许多内置类型需要初始化，可以将它们从初始化改为赋值，
// 并且可以提取到公共的函数中，供所有构造函数使用
// 当然，通过成员初值列完成真正的初始化更好
namespace B2 {
class B {
public:
    B(A& a): a_(a) {
        init_by_assignment();
    }
    B(A& a, int b): a_(a) {
        init_by_assignment(b);
    }
    B(A& a, int b, char c): a_(a) {
        init_by_assignment(b, c);
    }
private:
    void init_by_assignment(int b = 0, char c = '\n') {
        b_ = b;
        c_ = c;
    }
private:
    A a_;
    int b_;
    char c_;
};  // class B
}  // namespace B2

// part-3
// 初始化顺序: 先初始化 base classes，再按成员变量的声明初始化
namespace B3 {
namespace bad_case {
class B {
public:
    // 在初值列中，vec_ 依赖 n_ 的值，即使 n_ 的初始化写在 vec_ 前面，也是按照声明顺序进行初始化
    //  编译器 warning: field 'n_' is uninitialized when used here
    B(size_t num):
        n_(num / 2 + 1),
        // 为了防止 n_ 的值过大导致 vec_ 初始化很久，我们使用 std::min 来减少 vec_ 的初始化时间
        vec_((std::println("{}", n_), std::min((size_t)10, n_)), 2) {
    }
    void print() {
        PRINT_CURRENT_FUNCTION_NAME;
        std::println("vec_ size: {}", vec_.size());
        std::println("vec_: {}", vec_);
    }
private:
    std::vector<int> vec_;
    size_t n_;
};  // class B

/**
由于 vec_ 在 n_ 前面声明，vec_ 在初始化时 n_ 的值是一个随机数 6135853272 (即程序为 n_ 分配的内存所在地址的内存值)
@stdout:
6135853272
void B3::bad_case::B::print()
vec_ size: 10
vec_: [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
*/
void run() {
    B b(7);
    b.print();
}
}  // namespace bad_case

namespace solution {
class B {
public:
    B(size_t num):
        n_(num / 2 + 1),
        vec_((std::println("{}", n_), std::min((size_t)10, n_)), 2) {
    }
    void print() {
        PRINT_CURRENT_FUNCTION_NAME;
        std::println("vec_ size: {}", vec_.size());
        std::println("vec_: {}", vec_);
    }
private:
    // 由于 vec_ 的初始化依赖 n_，这里将 n_ 放到 vec_ 前声明
    size_t n_;
    std::vector<int> vec_;
};  // class B

/**
@stdout:
4
void B3::solution::B::print()
vec_ size: 4
vec_: [2, 2, 2, 2]
*/
void run() {
    B b(7);
    b.print();
    std::println();
}
}  // namespace solution
}  // namespace B3


// part-4
// 函数内的 static 称为 local static，其他的称为 non-local static
// C++ 对“定义域不同编译单元内的 non-local static 对象”的初始化相对次序并无明确定义
// @solution: 将 non-local static 对象搬到自己的专属函数内(可以放到类里做 static 函数，也可以放到类外)
/**
 编译单元 1：file_system
 编译单元 2：directory
 编译单元 3：main

 bad_case:
    编译单元 1 里有一个 non-local static FileSystem tfs
    编译单元 2 里有一个 non-local static Directory temp_dir
    temp_dir 在构造时使用了 tfs，但是 tfs 不一定在 temp_dir 构造前初始化
 solution:
    编译单元 1 里有 static FileSystem::TheFileSystem 函数，函数里面有一个 local static FileSystem tfs
    编译单元 2 里有 static Directory::TempDir 函数，函数里面有一个 local static Directory temp_dir
    temp_dir 构造时调用了 FileSystem::TheFileSystem 函数，函数第一次调用时会初始化 tfs，所以 tfs 一定在 temp_dir 前完成了构造
    即本条目的名称“确定对象被使用前已被初始化”

 在 more cpp idioms 也有类似的条款(construct on first use)，解决办法也是在 static class function 中加一个 local static 变量
 也可以说是以“函数调用”(返回一个 reference 指向 local static 对象)替换“直接访问 non-local static 对象”
 https://github.com/xuexcy/learning_more_cpp_idioms/blob/main/src/construct_on_first_use.cc
 */

namespace solution_for_non_local_static {

void run() {
    // bad
    bad_case::temp_dir.print_disk_serial_num();

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([]() {
            // good
            solution::Directory::TempDir().print_disk_serial_num();
        });
    }
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([]() {
            // good
            solution_2::temp_dir().print_disk_serial_num();
        });
    }
    std::for_each(threads.begin(), threads.end(), [](auto& thread) { thread.join(); });
}
}  // namespace solution_for_non_local_static


// part-5
// 在多线程环境下，使用 non-const static 对象可能会有些问题(多线程初始化 static 竞争), race conditions)
// @solution: 在程序启动阶段主动调用 reference-returning 函数(也就 part-4 中的 solution::Directory::TempDir()
// 和 solution_2::temp_dir())
void init_non_const_static() {
    solution::Directory::TempDir();
    solution_2::temp_dir();
}

int main() {
    B3::bad_case::run();
    B3::solution::run();

    // 先 init 以防有多线程使用这些 static 时发生错误
    init_non_const_static();
    solution_for_non_local_static::run();
    return 0;
}


