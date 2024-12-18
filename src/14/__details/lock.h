/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/18 17:04:08
# Desc   :
########################################################################
*/
#pragma once

#include <mutex>
#include <print>

inline void lock(std::mutex* mtx_ptr) {
  mtx_ptr->lock();
}
inline void unlock(std::mutex* mtx_ptr) {
  std::println("unlock");
  mtx_ptr->unlock();
}

