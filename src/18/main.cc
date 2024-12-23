/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/23 16:02:23
# Desc   :
########################################################################
*/

#include <print>
namespace bad_case {

class Date {
public:
  Date(int month, int day, int year): month_(month), day_(day), year_(year) {}
  void print() const {
    std::println("month: {}, day: {}, year: {}", month_, day_, year_);
  }
private:
  int month_;
  int day_;
  int year_;
};  // class Date

void run() {
  Date date(12, 23, 2024);
  Date date2(23, 12, 2024);
  date.print();
  date2.print();
}
}  // namespace bad_case

namespace solution {
struct Day {
  explicit Day(int d): d(d) {}
  int d;
};  // struct Day

struct Month {
private:
  explicit Month(int m): m(m) {}
public:
  static Month Jan() { return Month(1); }
  static Month Feb() { return Month(2); }
  static Month Mar() { return Month(3); }
  static Month Apr() { return Month(4); }
  static Month May() { return Month(5); }
  static Month Jun() { return Month(6); }
  static Month Jul() { return Month(7); }
  static Month Aug() { return Month(8); }
  static Month Sep() { return Month(9); }
  static Month Oct() { return Month(10); }
  static Month Nov() { return Month(11); }
  static Month Dec() { return Month(12); }
  int m;
};  // struct Month

struct Year {
  explicit Year(int y): y(y) {}
  int y;
};  // struct Year

class Date {
public:
  Date(const Month& month, const Day& day, const Year& year): month_(month), day_(day), year_(year) {}
  void print() const {
    std::println("month: {}, day: {}, year: {}", month_.m, day_.d, year_.y);
  }
private:
  Month month_;
  Day day_;
  Year year_;
};  // class Date

void run() {
  Date date(Month::Dec(), Day(23), Year(2024));
  date.print();
}

}  // namespace solution
int main() {
  bad_case::run();
  solution::run();
  return 0;
}

