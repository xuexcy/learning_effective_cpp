/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/25 16:39:04
# Desc    :   条款 08: 别让异常逃离析构函数(Prevent exceptions from leaving destructors)
########################################################################
*/

#include "cpp_utils/util.h"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <vector>

namespace bad_case {
class Widget {
public:
    Widget(): num_(new int(1)) {}
    ~Widget() noexcept(false) {
        PRINT_CURRENT_FUNCTION_NAME;
        throw std::runtime_error("destruct Widget error");
        delete num_;
    }
private:
    int* num_{nullptr};
};  // class Widget

// part-1
// C++ 并不禁止析构函数抛出异常，但不鼓励这样做
// 如下，v 在被销毁时，需要析构其中的 5 个 Widget，在析构第一个 Widget 抛出异常，程序终止，至少剩下的 4 个 Widget 没有被析构
// 假设程序没终止而是继续析构剩下的 4 个 Widget，那么就会积累更多没有被处理的异常，于程序而言异常太多了
// 即使不是容器销毁时这种异常累积的情况，对于 ~Widget() 抛出异常，也存在资源泄露的问题(delete a 没有被执行)
void run() {
    // std::vector<Widget> v(5);
}
}  // namespace bad_case

namespace solution  {
class DBConnection {
public:
    static DBConnection create() { return DBConnection(); }
    void close() {
        throw std::runtime_error("close DBConnection error");
    }
};  // class DBConnection

// part-2
// 对于可能发现的异常，我们可以既让用户知道并处理，又可以在用户忘记时替他处理
// 假设 class DBConnection 作为一个第三方工具库用于连接 db，并希望用户使用后主动 close(close 可能抛出异常)，

// solution_1:
// 将 class DBConnection 封装到 class DBConn，通过 ~DBConn() 来替用户主动 close(免得用户忘记)
// 但是 ~DBConn() 在调用 DBConnection::close() 时可能发生异常，我们不能让异常发生在析构函数中，于是，我们可以接住
// 异常后选择终止程序(std::abort) 或吞掉异常
class DBConn {
public:
    DBConn(const DBConnection& db): db_(db) {}
    ~DBConn() {
        try {
            db_.close();
        } catch (std::exception& e) {
            std::println("LOG[error]: db connection closed error, msg: {}", e.what());
            // std::abort();  // 可以选择终止程序或吞下异常
        }
    }
private:
    DBConnection db_;
};  // class DBConn

// solution_2:
// solution_1 并没有让用户感知 DBConnection 需要 close，并替用户处理了异常，我们可以在 DBConn 中添加一个close
// 接口给用户使用，并在用户忘记 close 时替他处理
class DBConnV2 {
public:
    DBConnV2(const DBConnection& db): db_(db) {}
    void close() {
        db_.close();
        close_ = true;
    }
    ~DBConnV2() {
        if (close_) {
            return;
        }
        try {
            db_.close();
        } catch (std::exception& e) {
            std::println("LOG[error]: db connection closed error, msg: {}", e.what());
            // std::abort();  // 可以选择终止程序或吞下异常
        }
    }
private:
    DBConnection db_;
    bool close_{false};
};  // class DBConnV2

void run() {
    {
        // 用户忘记了 close
        std::println("user forget close connection");
        DBConnection db;
    }
    {
        // 用户必须不能忘记 close，但如果忘记处理异常依然会导致程序终止
        std::println("user close and ignore exception");
        DBConnection db;
        try {
            db.close();
        } catch (...) {}
    }
    {
        // 用户不知道自己要 close，并且 class DBConn 隐藏并处理了异常
        std::println("user no need to close connection, DBConn close connection when call destructor and process the exception");
        DBConn db(DBConnection::create());
    }
    {
        // 用户选择自己 close 并处理了异常
        std::println("user close connection and process exception");
        DBConnV2 db(DBConnection::create());
        try {
            db.close();
        } catch (std::exception& e) {
            std::println("I am a DBConnection user, I get a exception, go ahead");
        }
    }
    {
        // 用户忘记了 close 并由 DBConnV2 代为 close 并处理异常
        std::println("user forget close connection and DBConnV2 does it and process exception for user");
        DBConnV2 db(DBConnection::create());
    }
}

}  // namespace solution
int main() {
    bad_case::run();
    solution::run();
    return 0;
}

