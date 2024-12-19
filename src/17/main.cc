/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/19 18:16:41
# Desc   :
########################################################################
*/

#include <exception>
#include <print>
#include <stdexcept>
#include "cpp_utils/util.h"

class Widget {
public:
  Widget() { PRINT_CURRENT_FUNCTION_NAME; }
  ~Widget() { PRINT_CURRENT_FUNCTION_NAME; }
};  // class Widget

int priority() {
  throw std::runtime_error("priority exception");
  return 1;
}

void process_widget(std::shared_ptr<Widget> widget, int priority) {
  std::println("priority {}", priority);
}

// part-1
// 编译器 process_widget(std::shared_ptr<Widget>(new Widget), priority()) 这个语句时，对于传入
// process_widget 的参数需要执行 3 个步骤
//   1. Widget* temp = new Widget;
//   2. std::shared_ptr<Widget>(temp);
//   3. priority()
// 其中 1 一定在 2 前面执行，但是 3 的执行顺序与 1、2 没有关系，如果编译器将 3 在 1、2 中间执行，那么 1 new 的资源
// 在 3 执行完抛出异常后，就无法通过 2 来释放资源
// 所以，对于申请资源并置于智能指针内这两个步骤，应该以独立语句来完成
void run() {
  {
    try {
      // bad
      process_widget(std::shared_ptr<Widget>(new Widget), priority());
    } catch (std::exception& e) {
      std::println("exception: {}", e.what());
    }
  }
  std::println();
  {
    try {
      // yes
      process_widget(std::make_shared<Widget>(), priority());
    } catch (std::exception& e) {
      std::println("exception: {}", e.what());
    }
  }
  std::println();
  {
    try {
      // yes
      std::shared_ptr<Widget> widget(new Widget);
      process_widget(widget, priority());
    } catch (std::exception& e) {
      std::println("exception: {}", e.what());
    }
  }

}
int main() {
  run();
  return 0;
}

