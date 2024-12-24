/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/23 17:20:31
# Desc   :
########################################################################
*/

#include <print>
#include <string>

#include "cpp_utils/util.h"

class Person {
public:
  Person() {
    PRINT_CURRENT_FUNCTION_NAME;
  }
  virtual ~Person() {
    PRINT_CURRENT_FUNCTION_NAME;
  }
private:
  std::string name_;
  std::string address_;
};  // class Person

class Student : public Person {
public:
  Student() {
    PRINT_CURRENT_FUNCTION_NAME;
  }
  virtual ~Student() {
    PRINT_CURRENT_FUNCTION_NAME;
  }
private:
  std::string school_name_;
  std::string school_address_;
};  // class Student

bool validate_student(const Student& student) {
  return true;
}

// part-1
// 通过 pass-by-value 会出现多余的构造和析构
// bad
// 6 次构造 + 6 次析构
// Student + Person + name_ + address_ + school_name_ + school_address_
bool validate_student_2(Student student) {
  return true;
}

class Window {
public:
  Window(const std::string& name): name_(name) {}
  std::string name() const { return name_; }
  virtual void display() const {
    PRINT_CURRENT_FUNCTION_NAME;
  }
private:
  std::string name_;
};  // class Window

class WindowWithScrollBars : public Window {
public:
  using Window::Window;
  virtual void display() const override {
    PRINT_CURRENT_FUNCTION_NAME;
  }
};  // class WindowWithScrollBars

void print_name_and_display(const Window& w) {
  std::println("name: {}", w.name());
  w.display();
}
// part-2
// 切割(slicing)问题
// WindowWithScrollBars 对象的所有特化信息都会被切除
void print_name_and_display_2(Window w) {
  std::println("name: {}", w.name());
  w.display();
}

void run() {
  Student student;
  validate_student(student);
  validate_student_2(student);

  Window window("window");
  WindowWithScrollBars window_with_scroll_bars("window_with_scroll_bars");
  print_name_and_display(window_with_scroll_bars);
  print_name_and_display(window_with_scroll_bars);

  print_name_and_display_2(window_with_scroll_bars);
  print_name_and_display_2(window_with_scroll_bars);
}
int main() {
  run();
  return 0;
}


