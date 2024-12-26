/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/26 20:56:17
# Desc   :
########################################################################
*/

#include <cstdint>
#include <string>

static uint32_t kMinimumLength = 8;
// part-1
// 变量定义后，可能没有使用，造成多余的构造与析构
// 要用时再定义变量
namespace bad_case {
std::string encrypt_password(const std::string& password) {
  std::string encrypted;
  if (password.length() < kMinimumLength) {
    throw std::logic_error("password is too short");
  }
  // xxx
  return encrypted;
}
}  // namespace bad_case

namespace solution {
std::string encrypt_password(const std::string& password) {
  if (password.length() < kMinimumLength) {
    throw std::logic_error("password is too short");
  }
  std::string encrypted;  // 要用时再定义
  // xxx
  return encrypted;
}
}  // namespace solution

// part-2
// 直接构造优于默认构造后赋值
void encrypt(std::string& password) {
  // xxx
}
namespace bad_case_2 {
std::string encrypt_password_2(const std::string& password) {
  if (password.length() < kMinimumLength) {
    throw std::logic_error("password is too short");
  }
  std::string encrypted;
  encrypted = password;  // 默认构造后赋值
  encrypt(encrypted);
  return encrypted;
}
}  // namespace bad_case_2

namespace solution_2 {
std::string encrypt_password_2(const std::string& password) {
  if (password.length() < kMinimumLength) {
    throw std::logic_error("password is too short");
  }
  std::string encrypted(password);  // 直接构造
  encrypt(encrypted);
  return encrypted;
}
}  // namespace solution_2

class Widget {
public:
  Widget() = default;
  Widget(int i) : i_(i) {}
  Widget& operator=(int i) {
    i_ = i;
    return *this;
  }

private:
  int i_{0};
};  // class Widget

// part-3
// 对于在 for 循环中使用的变量，在循环外定义还是在循环内定义
//   A. 定义在循环外
//   B. 定义在循环内
// 此时，需要考虑构造、析构、赋值的次数，如果构造和析构成本大于赋值，那么定义在循环外优于定义在循环内
// 另外，对于定义在 for 循环外的变量，会增加变量的作用域
// 赋值成本低 + 要求比较高的效率，则选择 A，否则选择 B
void run_something() {
  int n = 10;
  {
    // A. 1 次构造默认构造 + 1 次析构 + n 次拷贝赋值
    Widget w;
    for (int i = 0; i < n; ++i) {
      w = i;
    }
  }
  {
    // B. n 次构造 + n 次析构
    for (int i = 0; i < n; ++i) {
      Widget w(i);
    }
  }
}

int main() {
  return 0;
}
