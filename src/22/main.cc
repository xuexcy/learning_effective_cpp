/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/24 16:46:12
# Desc   :
########################################################################
*/

// part-1
// 将成员变量设置为 private，然后提供 public 的 get/set 方法
#include <cstdint>
#include <numeric>
#include <vector>

class AccessLevels {
public:
  int getReadOnly() const { return readOnly_; }

  void setReadWrite(int value) { readWrite_ = value; }
  int getReadWrite() { return readWrite_; }

  void setWriteOnly(int value) { writeOnly_ = value; }
private:
  int noAccess_;
  int readOnly_;
  int readWrite_;
  int writeOnly_;
};  // class AccessLevels

// part-2
// 通过函数访问成员变量，如果某个计算发生了变更，就只需要修改函数实现即可
// 将成员变量隐藏在函数接口的背后，可以为所有可能的实现提供弹性
class SpeedDataCollection {
public:
  void addValue(uint32_t speed) {
    sum_ += speed;
    ++cnt_;
  }
  inline double average() const {
    return sum_ * 1.0 / cnt_;
  }
private:
  uint32_t sum_;
  uint32_t cnt_;
};  // class SpeedDataCollection

class SpeedDataCollection2 {
public:
  void addValue(uint32_t speed) {
    speeds_.push_back(speed);
  }

  double average() const {
    return std::accumulate(speeds_.begin(), speeds_.end(), 0) * 1.0 / speeds_.size();
  }
private:
  std::vector<uint32_t> speeds_;
};  // class SpeedDataCollection2

// part-3
// 对于 protected 成员变量，当其发生改变，继承类中使用它的地方也要相应修改
// 因此，protected 成员变量也像 public 成员变量一样缺乏封装性
int main() {
  return 0;
}

