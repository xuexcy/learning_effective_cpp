/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/12/17 21:20:08
# Desc    :   条款 10： 令 operator= 返回一个 reference to *this(Have assignment operators return a reference to *this)
########################################################################
*/
void run_int_assignment() {
  int x, y, z;
  x = y = z = 15;
  // 等价于
  x = (y = (z = 15));
}

// part-1
// 对自定义类实现连锁

class Widget {
public:
  Widget& operator=(const Widget& rhs) {
    // copy something to this
    return *this;
  }
  Widget& operator+=(const Widget& rhs) {
    // add something to this
    return *this;
  }
};  // class Widget

void run() {
  Widget w1, w2, w3;
  w1 = w2 = w3;
}


int main() {
  run_int_assignment();
  run();
  return 0;
}

