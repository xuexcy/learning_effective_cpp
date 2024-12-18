/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/12/17 21:34:17
# Desc    :   条款 11： 在 operator= 中处理“自我赋值”(Handle assignment to self in operator=)
########################################################################
*/


#include <utility>
namespace bad_case_1 {
class Widget {
public:
  Widget(int a): a_(a) {}
  Widget& operator=(const Widget& rhs) {
    // 即使 this == &rhs 也没有什么影响
    this->a_ = rhs.a_;
    return *this;
  }
private:
  int a_{0};
};  // class Widget

void run() {
  Widget w1(10);
  // 无用但可以编译、无影响
  w1 = w1;
}
}  // namespace bad_case_1

// part-1
// 在 operator= 中如果不校验 this == &rhs，可能造成赋值的资源不存在
// 证同测试 identity test
namespace bad_case_2 {
class Widget {
public:
  Widget(int* a): a_(a) {}
  Widget& operator=(const Widget& rhs) {
    delete a_;
    // no: 当 this == &rhs 时，a_ 已经被 delete 了
    a_ = new int(*rhs.a_);
    return *this;
  }
private:
  int* a_{nullptr};
};  // class Widget

void run() {
  int* a = new int(3);
  Widget w1(a);
  // no: operator= 里会 delete w1.a_;
  // w1 = w1;
}
}  // namespace bad_case_2

namespace solution {
class Widget {
public:
  Widget(int* a): a_(a) {}
  Widget& operator=(const Widget& rhs) {
    // 和 bad_case_2 相比，多了如下一句比较
    if (this == &rhs) { return *this; }
    delete a_;
    a_ = new int(*rhs.a_);
    return *this;
  }
private:
  int* a_{nullptr};
};  // class Widget

void run() {
  int* a = new int(3);
  {
    Widget w1(a);
    // do nothing
    w1 = w1;
  }
}
}  // namespace solution

// part-2
// 在 solution 的 operator= 中，如果在 new 时发生异常，则原有的 a_ 会指向一段被 delete 的内存（因为先执行了 delete a_）
namespace solution_2 {
class Widget {
public:
  Widget(int* a): a_(a) {}
  Widget& operator=(const Widget& rhs) {
    if (this == &rhs) { return *this; }
    // 这里我们先将原有地址存下来，在保证 new 成功后再 delete
    // 上面即使不进行证同测试，也可以保证逻辑正确性，只是性能差一点
    // 即使没有证同测试，下面代码也会运行
    auto old = a_;
    a_ = new int(*rhs.a_);
    delete old;
    return *this;
  }
private:
  int* a_{nullptr};
};  // class Widget

void run() {
  int* a = new int(3);
  int* b = new int(4);
  Widget w1(a);
  Widget w2(b);
  w1 = w2;
}
}  // namespace solution_2

// part-3
// 针对 part-2 中的 存储 old -> 构造 new -> delete，可以用 copy_and_swap 技术来解决异常安全性问题
namespace solution_3 {
class Widget {
public:
  Widget(int* a): a_(a) {}
  // Widget& operator=(const Widget& rhs) {
  //   Widget temp(rhs);
  //   swap(temp);
  //   return *this;
  // }
  Widget& operator=(Widget rhs) {
    // https://github.com/xuexcy/learning_more_cpp_idioms/blob/main/src/copy_and_swap.cc#L140
    // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap
    // copy elision and copy-and-swap idiom
    swap(rhs);
    return *this;
  }
private:
  void swap(Widget& rhs) {
    std::swap(this->a_, rhs.a_);
  }
private:
  int* a_{nullptr};
};  // class Widget

void run() {
  int* a = new int(3);
  int* b = new int(4);
  Widget w1(a);
  Widget w2(b);
  w1 = w2;
}
}  // namespace solution_3

int main() {
  bad_case_1::run();
  bad_case_2::run();
  solution::run();
  solution_2::run();
  solution_3::run();
  return 0;
}
