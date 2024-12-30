/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/27 23:30:51
# Desc   :
########################################################################
*/

#include <format>
#include <memory>
#include <print>
#include <string>
#include <vector>

#include "cpp_utils/util.h"

class Widget {
public:
  explicit Widget(int size): size_(size) { PRINT_CURRENT_FUNCTION_NAME; }
private:
  int size_;
};  // class Widget

void do_some_work(const Widget& w) {
  // xxx
}

void run() {
  do_some_work(Widget(10));
  do_some_work(static_cast<Widget>(15));
}

class BaseA {
  int a_;
};
class BaseB {
  int b_;
};
class Derived_1: public BaseA {};
class Derived_2: public BaseA, public BaseB {};

// part-1
// 有时候 base 指针和 derived 指针的地址是不一样的，可能会存在一个偏移量
void run_base() {
  {
    Derived_1 d;
    BaseA* pb = &d;
    std::println("address_of(d): {}", (void*)(&d));
    std::println("address_of(pb): {}", (void*)(pb));
  }
  {
    Derived_2 d;
    BaseA* pa = &d;
    BaseB* pb = &d;
    std::println("address_of(d): {}", (void*)(&d));
    std::println("address_of(pa): {}", (void*)(pa));
    std::println("address_of(pb): {}", (void*)(pb));
  }
}

class Window {
public:
  Window(int width, int height): width_(width), height_(height) {}
  virtual void onResize(int width, int height) {
    width_ = width;
    height_ = height;
  }
  std::string to_string() {
    return std::format("width: {}, height: {}", width_, height_);
  }
private:
  int width_;
  int height_;
};  // class Window
namespace bad_case {
class SpecialWindow: public Window{
public:
  SpecialWindow(int width, int height, int color): Window(width, height), color_(color) {}
  virtual void onResize(int width, int height) override {
    // no:
    // 这里会产生一个临时对象，然后调用基类的 onResize，然后析构临时对象
    // 相当于 Window(*this).onResize(width, height);
    static_cast<Window>(*this).onResize(width, height);
  }
  std::string to_string() {
    return std::format("{}, color: {}", static_cast<Window>(*this).to_string(), color_);
  }
private:
  int color_;
};  // class SpecialWindow

/**
可以看到，调用 onResize 后，width、height 没有改变
stdout:
void bad_case::run()
sw before resize: width: 10, height: 20, color: 1
sw after resize: width: 10, height: 20, color: 1
*/
void run() {
  PRINT_CURRENT_FUNCTION_NAME;
  SpecialWindow sw(10, 20, 1);
  std::println("sw before resize: {}", sw.to_string());
  sw.onResize(1, 2);
  std::println("sw after resize: {}", sw.to_string());
}
}  // namespace bad_case

// part-2
// 在需要调用基类的函数时，不要使用 static_cast<Window>(*this)，而是直接调用 Window::onResize
namespace solution {
class SpecialWindow: public Window{
public:
  SpecialWindow(int width, int height, int color): Window(width, height), color_(color) {}
  virtual void onResize(int width, int height) override {
    // no:
    // 不要让编译器将 *this 当成一个 base 对象，而是希望调用 base 的一个函数
    // static_cast<Window>(*this).onResize(width, height);

    // yes:
    Window::onResize(width, height);
  }
  std::string to_string() {
    return std::format("{}, color: {}", static_cast<Window>(*this).to_string(), color_);
  }
  void hi() {
    PRINT_CURRENT_FUNCTION_NAME;
  }
private:
  int color_;
};  // class SpecialWindow

/**
@stdout:
void solution::run()
sw before resize: width: 10, height: 20, color: 1
sw after resize: width: 1, height: 2, color: 1
*/
void run() {
  PRINT_CURRENT_FUNCTION_NAME;
  SpecialWindow sw(10, 20, 1);
  std::println("sw before resize: {}", sw.to_string());
  sw.onResize(1, 2);
  std::println("sw after resize: {}", sw.to_string());
}
}  // namespace solution


// part-3
// dynamic_cast 的许多实现版本执行速度相当慢，至少有一个很普遍的实现版基于“class 名字之字符串比较”，如果你在
// 四层深的单继承体系内的某个对象上执行 dynamic_cast，那么就会调用四次 strcmp，用以比较 class 名字
// 在明确知道可以向下 dynamic_cast 的类型时，可以使用 static_cast，因为 static_cast 会更快

int main() {
  run();
  run_base();
  bad_case::run();
  solution::run();
  return 0;
}


