/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/18 18:59:15
# Desc   : 条款 15：在资源管理类中提供对原始资源的访问(Provide access to raw resources in resource-managing classes)
########################################################################
*/

#include <memory>
#include <print>
#include "__details/font_handle.h"
#include "font_handle.h"

void print(FontHandle f) {
  std::println("This is FontHandle");
}
// part-1
// 在我们将一些功能封装成 RAII 的类后，用户还是需要使用以原始资源的为参数的函数接口，我们需要在 RAII 类中提供这样的
// 接口让用户访问原始资源，比如智能指针的 get() 函数，我们也可以这样做，当然也可以使用隐式转换 operator Data() const
// 不过隐式转换可能会导致错误
void run() {
  Font font(get_font());
  print(font.get());  // 显示转换
  print(font);  // 隐式转换
  // 隐式转换会有问题，如果 fh 的使用超过了 font 的生命周期，比如 return fh，那 font 中的资源可能会被 delete
  // 如果是显示转换，我们就能更清楚的知道要控制生命周期
  // 另外，可能用户想要的是拷贝 Font，错写成了 FontHandle 导致了隐式转换
  FontHandle fh = font;  // 隐式转换
}

int main() {
  run();
  return 0;
}



