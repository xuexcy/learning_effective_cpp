/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/18 16:34:51
# Desc   : 条款 13：以对象管理资源(Use objects to manage resources)
########################################################################
*/

#include <memory>
class Investment {
public:
  static Investment* createInvestment() {
    return new Investment();
  }
};  // class Investment

namespace bad_case {
void foo() {
  Investment* inv = Investment::createInvestment();
  // 因为业务逻辑 while/go_to/if 提前 return
  // 资源泄露
  delete inv;
}
}  // namespace bad_case

// part-1
// 1. 获得资源后立刻放进管理对象
// 2. 管理对象运用析构函数确保资源被释放
// 如下代码的管理对象类型是 std::unique_ptr，利用其析构函数来释放 Investment
namespace solution {
void foo() {
  std::unique_ptr<Investment> inv(Investment::createInvestment());

  // delete inv after foo returned
}
}  // namespace solution

int main() {
  return 0;
}

