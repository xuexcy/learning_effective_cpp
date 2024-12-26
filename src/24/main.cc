/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/25 18:02:28
# Desc   : 条款 24：若所有参数皆需类型，请为此采用 non-member 函数(Declare non-member functions when type conversion should apply to all parameters)
########################################################################
*/

namespace bad_case {

class Rational {
public:
  Rational(int numerator = 0, int denominator = 1)
      : numerator_(numerator), denominator_(denominator) {
  }
  inline int n() const { return numerator_; }
  inline int d() const { return denominator_; }
  const Rational operator*(const Rational& rhs) const {
    return Rational(n() * rhs.n(), d() * rhs.d());
  }
private:
  int numerator_;
  int denominator_;
};  // class Rational

void run() {
  Rational a(1, 2);
  Rational b(2, 3);
  Rational result = a * b;  // yes, operator* + copy constructor
  result = a * b;  // yes, operator* + copy assignment operator
  // 如果构造函数是 explicit 的，下面的代码就会编译失败
  result = a * 2;  // yes, 隐式转换(implicit type conversion) Rational(2) + operator* + copy assignment operator
  // result = 2 * a;  // no
}
}  // namespace bad_case

namespace solution {
class Rational {
public:
  Rational(int numerator = 0, int denominator = 1)
      : numerator_(numerator), denominator_(denominator) {
  }
  inline int n() const { return numerator_; }
  inline int d() const { return denominator_; }
private:
  int numerator_;
  int denominator_;
};  // class Rational

const Rational operator*(const Rational& lhs, const Rational& rhs) {
  return Rational(lhs.n() * rhs.n(), lhs.d() * rhs.d());
}  // namespace solution

// part-1
// 如果参数（比如 operator* 的 lhs 或 rhs）都需要隐式转换，那么使用 non-member 函数来完成
void run() {
  Rational a(1, 2);
  Rational b(2, 3);
  Rational result = a * b;
  result = 2 * a;  // no  // yes
}

}  // namespace  solution

int main() {
  bad_case::run();
  solution::run();
  return 0;
}
