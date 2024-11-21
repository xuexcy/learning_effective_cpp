/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/15 15:08:23
# Desc    :   条款 02: 尽量以 const, enum, inline 替换 #define(Prefer consts, enums, and inlines to #defines)
########################################################################
*/

#include <cassert>
#include <print>
#include <string>

#include "game_player.h"

// part-1 const
// 编译器是看不到 ASPECT_RATIO 的，如果编译时报错，只会报 1.6 ，如果这个东西定义在其他地方然后编译时报错，你可能不知道
// 为什么会在错误中出现 "1.6" 这个错误信息
#define ASPECT_RATIO 1.6
// 用 const 替代 #define
const double AspectRatio = 1.6;

// part-2 const
// 如果类型是指针类型，则要写两个 const
// 因为指针指向的地址(即指针的值 ptr)和指针指向的地址的值(即 *ptr) 都不能改变
const char* const AuthorName = "he";
// 用 std::string 好一点
const std::string AuthorName2("he");

// part-3 enum
// 1. class 中的 static member 声明 + 定义分开
// 2. enum hack
// game_player.h

// part-4 template inline
// 宏展开可能会有坑
void f(int i) {}
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))
// 通过 template inline 解决问题
template <class T>
inline void call_with_max(const T& a, const T& b) {
    f (a > b ? a : b);
}
void run_call_with_max() {
    {
        int a = 5;
        int b = 0;
        // 以下调用展开为 f((++a) > (b) ? (++a) : (b))
        // 由于 (++a) == 6 > 0 == (b), 函数调用为 f((++a)) => f(7)
        // 于是 a 累加了 2 次
        CALL_WITH_MAX(++a, b);
        std::println("a = {}", a);
        assert(7 == a);
    }
    {
        int a = 5;
        int b = 0;
        // 以下调用展开为 f((++a) > (b) ? (++a) : (b))
        // 由于 (++a) == 6 < 10 == (b + 10), 于是函数调用为 f((b)) => f(0)
        // 于是 a 累加了 1 次
        CALL_WITH_MAX(++a, b + 10);
        std::println("a = {}", a);
        assert(6 == a);
    }
    {
        int a = 5;
        int b = 0;
        call_with_max(++a, b);
        std::println("a = {}", a);
        assert(6 == a);
    }
}
int main() {
    run_call_with_max();
    return 0;
}
