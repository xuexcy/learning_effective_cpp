/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/15 16:00:30
# Desc    :
########################################################################
*/

#include <cassert>
#include <format>
#include <print>

class Rational {
public:
    Rational(int a): a_(a) {}
    bool operator==(const Rational& rhs) const {
        return a_ == rhs.a_;
    }
    auto value() const { return a_; }
    int a_;
};

template <>
struct std::formatter<Rational> : std::formatter<std::string> {
  auto format(Rational r, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[a = {}]", r.a_), ctx);
  }
};

// part-1
// 对于返回类型为非内置类型,需要使用 const，以防对返回值进行赋值
namespace bad {
Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.a_ * rhs.a_);
}
void run() {
    Rational a(2);
    Rational b(3);
    Rational c(6);
    // a * b == c: true
    std::println("a * b == c: {}", a * b == c);
    Rational d(7);
    // a * b == c: [a = 7]
    std::println("a * b == c: {}", a * b = d);  // 不小心将 == 打成了 =，但还是可以编译，不符合我们的预期
}

}  // namespace bad
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.a_ * rhs.a_);
}

void run() {
    Rational a(2);
    Rational b(3);
    Rational c(6);
    // a * b == c: true
    std::println("a * b == c: {}", a * b == c);
    // 编译错误
    // std::println("compare a * b == c: {}", a * b = c);
    bad::run();
}

// part-2
// const 成员函数，bitwise constness 和 logical constness
// 通过 mutable 让 const 成员函数可以更改成员变量

// part-3
// 在 const 和 non-const 成员函数中避免重复
// 常量性转除 casting away constness
namespace part_3 {
class A {
public:
    const char& operator[](std::size_t position) const {
        // LOG_INFO("xxx")
        assert(position < s_.length());
        std::println("access position: {}, char: {}", position, s_[position]);
        return s_[position];
    }
    // char& operator[](std::size_t position) {
    //     // LOG_INFO("xxx")
    //     std::println("access position: {}, char: {}", position, s_[position]);
    //     assert(position < s_.length());
    //     return s_[position];
    // }
    char& operator[](std::size_t position) {
        // 在已经存在 const 成员函数的情况下
        // 1. 先用 static_cast 将 *this 转换成 const A&
        // 2. 调用 const 成员函数
        // 3. 通过 const_cast 将函数调用后的结果去掉 const
        return const_cast<char&>(static_cast<const A&>(*this)[position]);
    }
    std::string s_;
};
}  // namespace part_3

int main() {
    run();
    return 0;
}


