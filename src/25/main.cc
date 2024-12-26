/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/25 22:40:32
# Desc   : 条款 25：考虑写一个不抛异常的 swap 函数(Consider support for a non-throwing swap)
########################################################################
*/
// https://github.com/xuexcy/learning_more_cpp_idioms/blob/main/src/non_throwing_swap.cc

#include <vector>

#include "cpp_utils/util.h"

// part-1
// 当 T 很复杂时，构造和拷贝 T 的实例开销都很大
namespace bad_case {
template <typename T>
void swap(T& a, T& b) {
  T tmp = a;  // copy constructor
  a = b;  // copy assignment operator
  b = tmp;  // copy assignment operator
}

}  // namespace bad_case


// part-2
// 通过 pImpl 将数据封装起来，在 swap 时交换 Impl 指针可以避免构造和拷贝的开销
// pImpl + public swap 函数 + std::swap 全特化
namespace solution {
class WidgetImpl {
public:
  WidgetImpl(int a, int b, int c) : a_(a), b_(b), c_(c) {}
private:
  int a_;
  int b_;
  int c_;
  std::vector<double> v_;
};  // class WidgetImpl

class Widget {
public:
  Widget(const Widget& rhs): impl_(new WidgetImpl(*rhs.impl_)) {}
  Widget& operator=(const Widget& rhs) {
    *impl_ = *rhs.impl_;
    return *this;
  }
  void swap(Widget& rhs) {
    std::swap(impl_, rhs.impl_);
  }
private:
  WidgetImpl* impl_;
};  // class Widget

}  // namespace solution

namespace std {
// 全特化 std::swap
template <>
void swap<solution::Widget>(solution::Widget& a, solution::Widget& b) {
  a.swap(b);
}
}

namespace bad_case_2 {
template <typename T>
class WidgetImpl {
public:
  WidgetImpl(T a, T b, T c) : a_(a), b_(b), c_(c) {}
private:
  T a_;
  T b_;
  T c_;
  std::vector<double> v_;
};  // class WidgetImpl

template <typename T>
class Widget {
public:
  Widget(const Widget& rhs): impl_(new WidgetImpl(*rhs.impl_)) {}
  Widget& operator=(const Widget& rhs) {
    *impl_ = *rhs.impl_;
    return *this;
  }
  void swap(Widget& rhs) {
    std::swap(impl_, rhs.impl_);
  }
private:
  WidgetImpl<T>* impl_;
};  // class Widget

}  // namespace bad_case_2

// part-3
// 1. C++标准并不支持函数模板偏特化
// 2. 一般可以重载 function template，但是在 namespace std，只能全特化，不能添加新的 templates
// 下面代码偏特化无法编译: Function template partial specialization is not allowed
namespace std {
// template <typename T>
// void swap<bad_case_2::Widget<T>>(bad_case_2::Widget<T>& a, bad_case_2::Widget<T>& b) {
//   a.swap(b);
// }
}

// part-4
// 对于无法偏特化的情况，我们可以声明 non-member 模板 swap 函数，然后在使用时使用 using std::swap; swap(xx, xx)
namespace solution_2 {
template <typename T>
class WidgetImpl {
public:
  WidgetImpl(T a, T b, T c) : a_(a), b_(b), c_(c) {}
private:
  T a_;
  T b_;
  T c_;
  std::vector<double> v_;
};  // class WidgetImpl


template <typename T>
class Widget {
public:
  Widget(): impl_(new WidgetImpl<T>(T(), T(), T())) {}
  Widget(const Widget& rhs): impl_(new WidgetImpl(*rhs.impl_)) {}
  Widget& operator=(const Widget& rhs) {
    *impl_ = *rhs.impl_;
    return *this;
  }
  ~Widget() {
    if (impl_ != nullptr) { delete impl_; }
  }
  void swap(Widget& rhs) {
    // 这里最好也不要直接使用 std::swap，因为后续添加了一些其他类型，也可以使用 named-lookup-rules 来查找 swap 函数
    // std::swap(impl_, rhs.impl_);
    using std::swap;
    swap(impl_, rhs.impl_);
  }
private:
  WidgetImpl<T>* impl_;
};  // class Widget

template <typename T>
void swap(Widget<T>& a, Widget<T>& b) {
  PRINT_CURRENT_FUNCTION_NAME;
  a.swap(b);
}
}  // namespace solution


void run_std_swap() {
  PRINT_CURRENT_FUNCTION_NAME;
  solution_2::Widget<int> a;
  solution_2::Widget<int> b;
  // 这里使用的 std::swap，会发生构造和拷贝，就像 bad_case::swap 一样
  std::swap(a, b);
}

void run_solution_2_swap() {
  PRINT_CURRENT_FUNCTION_NAME;
  solution_2::Widget<int> a;
  solution_2::Widget<int> b;
  // 如果使用 std::swap， 且 std::swap 不能全特化 Widget<T>，那么就会使用默认的 std::swap，发生三次拷贝与构造
  // 所以代码不要写成 std::swap(impl_, rhs.impl_)，而是使用 using std::swap; swap(xx, xx)
  using std::swap;
  // C++ 的名称查找法则确保将找到 global 作用域或 T 所在之命名空间内的任何 T 的专属 swap。
  // using std::swap 让该 swap 在函数内曝光
  // 命名查找法则(named lookup rules)，先在当前作用域，再到 global 作用域查找，最后到 std 查找
  swap(a, b);
}

void run() {
  run_std_swap();
  run_solution_2_swap();
}
int main() {
  run();
  return 0;
}
