/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyunxue@gmail.com
# Date    :   2024/12/19 16:52:14
# Desc    :
########################################################################
*/
#pragma once

#include "__details/font_handle.h"
class Font {
public:
  explicit Font(FontHandle f): f_(f) {}
  ~Font() { release_font(f_); }
  FontHandle get() const { return f_; }  // 显示转换
  operator FontHandle() const { return f_; }  // 隐式转换
private:
  FontHandle f_;
};  // class Font
