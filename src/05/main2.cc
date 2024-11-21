/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/21 17:04:26
# Desc    :   条款 05: 了解 C++ 默默编写并调用了哪些函数(Know what functions C++ silently writes and calls)
########################################################################
*/

#include <string>

// part-2
// 对于编译器创建 的 copy 构造函数和 copy assignment 操作符，只是将 non-static 成员变量拷贝到目标对象
// 拷贝时，对于非内置类型(比如 std:string)，使用 copy 构造函数，对于内置类型(比如 int)直接按 bits 拷贝
template <class T>
class NamedObject {
public:
    NamedObject(const char* name, const T& value): name_value_(name), object_value_(value) {}
    NamedObject(const std::string& name, const T& value): name_value_(name), object_value_(value) {}
private:
    std::string name_value_;
    T object_value_;
};  // class NamedObject

void run_copy() {
    NamedObject<int> obj_1("hi", 2);
    // copy constructor
    // 对 NamedObject<int>::name_value_ 使用 std::string 的 copy constructor
    // 对 NamedObject<int>::object_value 这个内置类型 int，直接拷贝 obj_1.object_value_ 的各个 bits 到 obj_2.object_value_
    decltype(obj_1) obj_2(obj_1);
}

// part-3
// reference 无法被赋值, const 无法被修改
// 所以编译器拒绝生成 copy assignment operator 并拒绝编译赋值动作
// 另外，如果 base class 将 copy assignment operator 声明为 private，编译器也会拒绝生成 derived class 的
// copy assignment operator
namespace reject_assignment_to_const_reference {
template <class T>
class NamedObject {
public:
    NamedObject(std::string& name, const T& value): name_value_(name), object_value_(value) {}
private:
    std::string& name_value_;  // reference
    const T object_value_;  // const

};  // class NamedObject

void run() {
    std::string new_dog("A");
    std::string old_dog("B");
    NamedObject<int> p(new_dog, 1);
    NamedObject<int> s(old_dog, 2);
    // Object of type 'NamedObject<int>' cannot be assigned because its copy assignment operator is implicitly deleted
    // 编译器拒绝编译赋值动作
    // p = s;
}

}  // namespace reject_assignment_to_const_reference
int main() {
    run_copy();
    reject_assignment_to_const_reference::run();
    return 0;
}
