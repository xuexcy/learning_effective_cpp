/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/24 21:37:12
# Desc    :   条款 07: 为多态基类声明 virtual 析构函数(Declare destructor virtual in polymorphic base classes)
########################################################################
*/

#include <random>
#include <sys/syslimits.h>

#include "cpp_utils/util.h"


// part-1
// class 含有 non-virtual destructor, 当被继承后再通过基类指针调用析构函数，存在“局部销毁”的问题。
// 通常 derived 类的成员变量可能没有被销毁，derived 类的析构函数也不会被执行
namespace bad_case {
class TimeKeeper {
public:
    TimeKeeper() { PRINT_CURRENT_FUNCTION_NAME; }
    ~TimeKeeper() { PRINT_CURRENT_FUNCTION_NAME; std::println(); }
};  // class TimeKeeper

class AtomicClock : public TimeKeeper {
public:
    AtomicClock() { PRINT_CURRENT_FUNCTION_NAME; }
    ~AtomicClock() { PRINT_CURRENT_FUNCTION_NAME; std::println(); }
};  // class AtomicClock
class WaterClock : public TimeKeeper {
public:
    WaterClock() { PRINT_CURRENT_FUNCTION_NAME; }
    ~WaterClock() { PRINT_CURRENT_FUNCTION_NAME; std::println(); }
};  // class WaterClock
class WristClock : public TimeKeeper {
public:
    WristClock() { PRINT_CURRENT_FUNCTION_NAME; }
    ~WristClock() { PRINT_CURRENT_FUNCTION_NAME; std::println(); }
};  // class WristClock

TimeKeeper* get_time_keeper() {
    static int n{0};
    ++n;
    n %= 3;
    if (0 == n) {
        return new AtomicClock();
    } else if (1 == n) {
        return new WaterClock();
    } else {
        return new WristClock();
    }
}

void run() {
    {
        /**
        下面通过 clock 超出作用域析构时，只调用了 AtomicClock 的析构函数
        @stdout:
        bad_case::TimeKeeper::TimeKeeper()
        bad_case::AtomicClock::AtomicClock()
        bad_case::AtomicClock::~AtomicClock()
        */
        AtomicClock clock;
    }
    for (int i = 0; i < 6; ++i) {
        /**
        下面通过delete 删除实例时，只输出了 TimeKeeper 的析构函数，没有继承类的析构函数
        @stdout:
        bad_case::TimeKeeper::TimeKeeper()
        bad_case::AtomicClock::AtomicClock()
        I get a TimeKeeper and then I will delete it
        bad_case::TimeKeeper::~TimeKeeper()
         */
        TimeKeeper* tk = get_time_keeper();
        std::println("I get a TimeKeeper and then I will delete it");
        // 这个 delete 不会调用 derived class 的析构函数
        delete tk;  // 局部删除(销毁 TimeKeeper 这一部分，而不删除继承类中的部分)
    }
}

}  // namespace bad_case

// part-2
// 当 class 中含有 virtual 函数时，class 实例中带有一个 vptr (virtual table pointer) 指向 vtbl (virtual table)
// 这样 sizeof(class) == sum_all(sizeof(class member variable)) + sizeof(vptr)，就是类大小增大一个 vptr 的大小

// 1. class 中含有 vtbl (一个由函数指针构成的数组)
// 2. class 实例中含有 vptr
// 3. 当实例调用虚函数时，实际调用哪个函数取决于 vptr 指向哪个 vtbl
namespace class_with_vptr {
// 这个 Point 的大小是 8，对于 64 位的机器，一个实例可以直接放到 64-bit 缓存器中，或者传递给其他语言，比如 C 语言
// 然而，当多了一个 vptr 后就不是这样了(C 语言没有虚表指针，一个 Point 包含两个 int 在 C++ 和 C 之间没法直接传递，因为 sizeof 不一样, 内存结构不一样)
struct Point {
public:
    Point(int x, int y): x_(x), y_(y) {}
    ~Point() = default;
private:
    int x_{0};
    int y_{0};
};  // struct Point
// sizeof(int) == 4
static_assert(2 * sizeof(int) == sizeof(Point));  // 8

struct PointV2 {
public:
    PointV2(int x, int y): x_(x), y_(y) {}
    virtual ~PointV2() = default;
private:
    int x_{0};
    int y_{0};

};  // struct PointV2
// sizeof(int) == 4 ,
// sizeof(void*)  == 8 , 64 位机器指针大小为 8，也就是 vptr (virtual table pointer) 大小
static_assert(2 * sizeof(int) + sizeof(void*) == sizeof(PointV2));  // 16 (64 位机器)

void run() {
    std::println("sizeof(Point): {}", sizeof(Point));
    std::println("sizeof(PointV2) with virtual table pointer: {}", sizeof(PointV2));
}
}  // namespace class_with_vptr

// part-3
// 即使时纯虚 析构函数，也需要完整定义，因为继承类的析构函数会调用基类的析构函数
namespace class_with_pure_virtual {
class Base {
public:
    virtual void print() { PRINT_CURRENT_FUNCTION_NAME; }
    virtual ~Base() = 0;
};  // class Base

/**  如果注释掉 ~Base() 的定义，将会出现链接错误，如下是 clang++ link error
Undefined symbols for architecture arm64:
  "class_with_pure_virtual::Base::~Base()", referenced from:
      class_with_pure_virtual::Derive::~Derive() in main.cc.o
ld: symbol(s) not found for architecture arm64
*/
Base::~Base() { PRINT_CURRENT_FUNCTION_NAME; }

class Derive : public Base {
public:
    void print() override {
        PRINT_CURRENT_FUNCTION_NAME;
    }
    ~Derive() override {
        PRINT_CURRENT_FUNCTION_NAME;
    }
};  // class Derive

void run() {
    Base* b = new Derive();
    b->print();
    delete b;
}
}  // namespace class_with_pure_virtual

int main() {
    bad_case::run();
    class_with_vptr::run();
    class_with_pure_virtual::run();
    return 0;
}
