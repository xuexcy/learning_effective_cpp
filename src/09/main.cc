/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/25 19:11:32
# Desc    :   条款 09: 绝对在构造和析构过程中调用 virtual 函数(Never call virtual functions during construction or destruction)
########################################################################
*/

#include "cpp_utils/util.h"

namespace bad_case {

// base class for all transaction
class Transaction {
public:
    Transaction() {
        // 编译提示
        // warning: call to pure virtual member function 'logTransactionPureVirtual' has undefined behavior; overrides of 'logTransactionPureVirtual' in subclasses are not available in the constructor of 'bad_case::Transaction'
        // 代码在执行到这里时退出: libc++abi: Pure virtual function called!
        // logTransactionPureVirtual();

        logTransaction();
        std::println("construct base class Transaction finished");
    }
    virtual void logTransactionPureVirtual() const = 0;  // log for different transaction types
    virtual void logTransaction() const {
        PRINT_CURRENT_FUNCTION_NAME;
    }
    virtual ~Transaction() {
        PRINT_CURRENT_FUNCTION_NAME;
        logTransaction();
        std::println("destruct base class Transaction finished");
    }
};  // class Transaction

class BuyTransaction : public Transaction {
public:
    BuyTransaction(double price): price_(price) {}
    virtual void logTransactionPureVirtual() const override {
        PRINT_CURRENT_FUNCTION_NAME;
        std::println("price: {}", price_);
    }
    virtual void logTransaction() const override {
        PRINT_CURRENT_FUNCTION_NAME;
        std::println("price: {}", price_);
    }
private:
    double price_{0.0};
};  // class BuyTransaction

class SellTransaction : public Transaction {
    virtual void logTransactionPureVirtual() const override {
        PRINT_CURRENT_FUNCTION_NAME;
    }
    virtual void logTransaction() const override {
        PRINT_CURRENT_FUNCTION_NAME;
    }
};  // class SellTransaction

// part-1
// base class 构造函数中调用的虚函数不会降到 derived class 中
// 在 derived class 对象在 base class 构造期间，对象的类型时 base class 而不是 derived class
/**
BuyTransaction 在构造实例时，会先构造 Transaction 部分，假设 Transaction 在构造函数中调用的虚函数可以指向
BuyTransaction 中的虚函数实现，而在这个 derived class 实现的虚函数中使用了成员变量 price_，那么行为将是未定义的，
因为继承类 BuyTransaction 中的成员变量 price_ 还没有完成初始化。 继承类构造顺序如下：
base class 成员初始化 -> base class 构造函数 -> derived class 成员初始化 -> derived class 构造函数
@stdout:
virtual void bad_case::Transaction::logTransaction() const
construct base class Transaction finished
virtual bad_case::Transaction::~Transaction()
virtual void bad_case::Transaction::logTransaction() const
destruct base class Transaction finished
*/
void run() {
    // 在 mac clang version 19.1.3 arm64-apple-darwin24.1.0 版本中
    // buy 在构造时
    // 如果基类构造函数中调用了纯虚函数，那么程序会退出
    // 如果基类构造函数中调用了虚函数，那么调用的是基类定义的虚函数，而不是继承类实现的虚函数
    BuyTransaction buy(1.8);
}
}  // namespace bad_case

// part-2
// 对析构函数而言，一旦 derived class 析构函数开始执行，对象内的 derived class 成员变量便呈现为定义值
// 在进入 base class 的析构函数时，对象也会被当成 base class 的实例


// part-3
// 首先，不要在基类构造函数中调用虚汗数，其次，对于需要在基类构造函数中需要使用继承类的一些信息时，可以通过由继承类构造时，
// 将比较的信息传递给基类构造函数来实现
// 如下 Transaction 的构造参数中的 log_info，可由 BuyTransaction 在构造时传递给 Transaction
// 同时，这些信息最好是由 static 函数产生，以防止普通成员函数生成这些信息时，使用未完成构造的成员变量
namespace solution {
class Transaction {
public:
    Transaction(const std::string& log_info) {
        logTransaction(log_info);
        std::println("construct base class Transaction finished");
    }
    void logTransaction(const std::string& log_info) const {
        PRINT_CURRENT_FUNCTION_NAME;
        std::println("transaction log info: {}", log_info);
    }
    virtual ~Transaction() {
        PRINT_CURRENT_FUNCTION_NAME;
        // logTransaction();
        std::println("destruct base class Transaction finished");
    }
};  // class Transaction

class BuyTransaction : public Transaction {
public:
    BuyTransaction(double price): Transaction(createLogString(price)), price_(price) {}
    void logTransaction() const {
        PRINT_CURRENT_FUNCTION_NAME;
        std::println("price: {}", price_);
    }
private:
    static std::string createLogString(double price) {
        return std::format("price: {}", price);
    }
private:
    double price_{0.0};
};  // class BuyTransaction

/**
@stdout:
void solution::Transaction::logTransaction(const std::string &) const
transaction log info: price: 1.8
construct base class Transaction finished
virtual solution::Transaction::~Transaction()
destruct base class Transaction finished
 */
void run() {
    BuyTransaction buy(1.8);
}

}  // namespace solution
int main() {
    bad_case::run();
    solution::run();
    return 0;
}
