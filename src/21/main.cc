/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/24 16:17:47
# Desc   : 条款 21：必须返回对象时，别妄想返回其 reference(Don't try to return a reference when you must return an object)
########################################################################
*/

#include "cpp_utils/util.h"

class Rational {
public:
  Rational(int numerator = 0, int denominator = 1)
      : numerator_(numerator), denominator_(denominator) {
    PRINT_CURRENT_FUNCTION_NAME;
  }
  inline int n() const { return numerator_; }
  inline int d() const { return denominator_; }
private:
  int numerator_;
  int denominator_;
};  // class Rational

const Rational operator*(const Rational& lhs, const Rational& rhs) {
  return Rational(lhs.n() * rhs.n(), lhs.d() * rhs.d());
}

void run() {
  PRINT_CURRENT_FUNCTION_NAME;
  Rational a(1, 2);
  Rational b(2, 3);
  // 一次构造 + copy constructor(当然，编译器可能有 copy elision)
  Rational c = a * b;
  // 两次构造 + copy constructor
  Rational d = a * b * c;
}

int main() {
  run();
  return 0;
}



