/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyunxue@gmail.com
# Date    :   2024/12/19 16:50:18
# Desc    :
########################################################################
*/
#pragma once

#include <print>
struct FontHandle {
};  // struct FontHandle

inline FontHandle get_font() { return FontHandle(); }
inline void release_font(FontHandle f) { std::println("release FontHandle"); }
