/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/18 16:59:31
# Desc   :
########################################################################
*/

#include <mutex>
#include <print>

#include "__details/lock.h"
#include "cpp_utils/util.h"
#include "lock.h"
#include "uncopyable.h"

namespace bad_case {
void run() {
  std::println();
  PRINT_CURRENT_FUNCTION_NAME;
  std::mutex mtx;
  lock(&mtx);
  std::println("lock mtx");
  unlock(&mtx);
}
}  // namespace bad_case

// part-1
// 对于一些 C 类型的 API 函数，我们可以将其封装到资源管理类当中，比如用 lock.h 中的 class Lock 来封装来封装
// __details/lock.h 中的 lock 和 unlock 函数，其中 unlock 函数需要用户主动调用。
//  1. 用户可能忘记调用 unlock
//  2. 在条款 13 中可以看到，代码片段有可能提前 return 导致无法执行用户调用的 unlock，我们需要它在析构时主动 unlock
//  3. 另外，我们还可以使用 https://github.com/xuexcy/learning_more_cpp_idioms/blob/main/src/function_poisoning.cc
//    来禁用这些 C likely 的 API(#pragma GCC poison lock)
namespace solution {
void run() {
  std::println();
  PRINT_CURRENT_FUNCTION_NAME;
  std::mutex mtx;
  Lock lock(&mtx);
  std::println("lock mtx");
}
}  // namespace solution

// part-2
// 如果 Lock 是可以复制的，那两个 Lock 在函数结束后都会执行析构函数中的 unlock，很明显不应该这样
// 复制 RAII 对象是不合理的，应该禁止其 copy（条款 06）
// solution:
//   1. 禁止赋值
//   2. 如果真的要复制，使用 std::shared_ptr，并将 deleter 设置成 unlock，因为常规使用的 std::shared_ptr 在计数器
//     归 0 时会删除资源（delete ptr）

namespace bad_case_duplicate_unlock {
void run() {
  std::println();
  PRINT_CURRENT_FUNCTION_NAME;
  std::mutex mtx;
  Lock lock(&mtx);
  std::println("lock mtx");
  Lock lock2(lock);
}
}  // namespace bad_case_duplicate_unlock

namespace solution_of_uncopyable {
void run() {
  std::println();
  PRINT_CURRENT_FUNCTION_NAME;
  using Lock = UncopyableLock;
  std::mutex mtx;
  Lock lock(&mtx);
  std::println("lock mtx");
  // UncopyableLock 不允许 copy
  // Lock lock2(lock);
}
}  // namespace solution_of_uncopyable

namespace solution_of_shared {
void run() {
  std::println();
  PRINT_CURRENT_FUNCTION_NAME;
  using Lock = SharedLock;
  std::mutex mtx;
  Lock lock(&mtx);
  std::println("lock mtx");
  // 由于 SharedLock 的构造函数在初始化 std::shared_ptr 时，指定了 deleter 为 unlock 函数，所以计数器归 0 时会 unlock，而不是删除资源 mtx
  Lock lock2(lock);
}
}  // namespace solution_of_shared


int main() {
  bad_case::run();
  solution::run();
  bad_case_duplicate_unlock::run();
  solution_of_uncopyable::run();
  solution_of_shared::run();
  return 0;
}
