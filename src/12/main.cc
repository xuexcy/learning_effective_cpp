/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/18 14:05:50
# Desc   : 条款 12: 复制对象时勿忘其每一个成分(Copy all parts of object)
########################################################################
*/

#include <print>
#include <string>

void logCall(const std::string& funcName) {
  std::println("LOG_INFO: {}", funcName);
}

class Customer {
public:
  Customer() = default;
  Customer(const std::string& name): name_(name) {}
  Customer(const Customer& rhs): name_(rhs.name_) {
    logCall("Customer copy constructor");
  }
  Customer& operator=(const Customer& rhs) {
    logCall("Customer copy assignment operator");
    name_ = rhs.name_;
    return *this;
  }
  std::string to_string() const {
    return std::format("[name: {}]", name_);
  }
private:
  std::string name_;
};  // class Customer

// part-1
// 当 Customer 新增一个 member variable 时，很容易忘记同步修改普通构造函数、拷贝构造函数、拷贝赋值操作符
// 所以，新添成员变量时，必须同时修改 copy 函数
class Customer2 {
public:
  Customer2() = default;
  Customer2(const std::string& name, int age): name_(name), age_(age) {}
  Customer2(const Customer2& rhs): name_(rhs.name_), age_(rhs.age_) {
    logCall("Customer2 copy constructor");
  }
  Customer2& operator=(const Customer2& rhs) {
    logCall("Customer2 copy assignment operator");
    name_ = rhs.name_;
    age_ = rhs.age_;
    return *this;
  }
  std::string to_string() const {
    return std::format("[name: {} , age: {}]", name_, age_);
  }
private:
  std::string name_;
  // 当新添 age_ 时，需要同步修改一些函数，但开发者可能忘记，而且也比较麻烦
  int age_{0};
};  // class Customer2

namespace bad_case {
// part-2
// 当发生继承时，拷贝构造、拷贝赋值操作符的实现也需要对基类中的成员变量进行拷贝，但开发时很可能忘记
// 另外，base class 中需要拷贝的 member variable 往往是 private 的，导致即使 derived class 想要一个个拷贝也比较困难
class PriorityCustomer : public Customer {
public:
  PriorityCustomer(const std::string& name, int priority): Customer(name), priority_(priority) {}
  PriorityCustomer(const PriorityCustomer& rhs):
      // Customer(rhs),  // 如果不主动调用 base class 的 copy constructor，编译器会帮助自动调用 base class 的无参构成函数
      priority_(rhs.priority_) {
    logCall("bad_case::PriorityCustomer copy constructor");
  }
  PriorityCustomer& operator=(const PriorityCustomer& rhs) {
    logCall("bad_case::PriorityCustomer copy assignment operator");
    // 这里遗漏了调用 base class 的 copy assignment operator
    priority_ = rhs.priority_;
    return *this;
  }
  std::string to_string() const {
    return std::format("{}, [priority: {}]", Customer::to_string(), priority_);
  }
private:
  int priority_{0};
};  // class PriorityCustomer
}  // namespace bad_case

namespace solution {
class PriorityCustomer : public Customer {
public:
  PriorityCustomer(const std::string& name, int priority): Customer(name), priority_(priority) {}
  PriorityCustomer(const PriorityCustomer& rhs): Customer(rhs), priority_(rhs.priority_) {
    logCall("solution::PriorityCustomer copy constructor");
  }
  PriorityCustomer& operator=(const PriorityCustomer& rhs) {
    // 主动调用 base class 的 copy assignment operator
    Customer::operator=(rhs);
    logCall("solution::PriorityCustomer copy assignment operator");
    priority_ = rhs.priority_;
    return *this;
  }
  std::string to_string() const {
    return std::format("{}, [priority: {}]", Customer::to_string(), priority_);
  }
private:
  int priority_{0};
};  // class PriorityCustomer
}  // namespace solution

void run() {
  {
    std::println("Customer");
    Customer a("a");
    std::println("a: {}", a.to_string());
    Customer b("b");
    std::println("b: {}", b.to_string());
    Customer c(a);
    std::println("c: {}", c.to_string());
    c = b;
    std::println("c: {}", c.to_string());
    std::println();
  }
  {
    std::println("Customer2");
    Customer2 a("a", 1);
    std::println("a: {}", a.to_string());
    Customer2 b("b", 2);
    std::println("b: {}", b.to_string());
    // 如果添加 age_ 后忘记修改 copy 函数，就会忘记 copy age_ from rhs
    Customer2 c(a);
    std::println("c: {}", c.to_string());
    c = b;
    std::println("c: {}", c.to_string());
    std::println();
  }
  {
    std::println("bad_case::PriorityCustomer");
    bad_case::PriorityCustomer a("a", 1);
    std::println("a: {}", a.to_string());
    bad_case::PriorityCustomer b("b", 2);
    std::println("b: {}", b.to_string());
    // 忘记拷贝 Customer 中的 name_，如果 Customer 还有很多其他 member variable，那也要一个个拷贝，比较麻烦
    bad_case::PriorityCustomer c(a);
    // stdout: c: [name: ], [priority: 1]
    std::println("c: {}", c.to_string());
    c = b;
    // stdout: c: [name: ], [priority: 2]
    std::println("c: {}", c.to_string());
    std::println();
  }
  {
    std::println("solution::PriorityCustomer");
    solution::PriorityCustomer a("a", 1);
    std::println("a: {}", a.to_string());
    solution::PriorityCustomer b("b", 2);
    std::println("b: {}", b.to_string());
    // 在 copy 函数中调用了基类的 copy 函数
    // Customer(rhs)
    solution::PriorityCustomer c(a);
    std::println("c: {}", c.to_string());
    // Customer::operator(rhs)
    c = b;
    std::println("c: {}", c.to_string());
  }
}

int main() {
  run();
  return 0;
}



