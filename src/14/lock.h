/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/18 17:09:48
# Desc   :
########################################################################
*/
#pragma once

#include "__details/lock.h"
#include "uncopyable.h"
#include <memory>
#include <mutex>

class Lock {
public:
  explicit Lock(std::mutex* mtx_ptr): mtx_ptr_(mtx_ptr) {
    lock(mtx_ptr_);
  }
  ~Lock() {
    unlock(mtx_ptr_);
  }
private:
  std::mutex* mtx_ptr_;
};  // class Lock

class UncopyableLock : public Lock, public Uncopyable {
  using Lock::Lock;
};  // UncopyableLock

class SharedLock {
public:
  explicit SharedLock(std::mutex* mtx_ptr): mtx_ptr_(mtx_ptr, unlock) {}
private:
  std::shared_ptr<std::mutex> mtx_ptr_{nullptr};
};  // class SharedLock
