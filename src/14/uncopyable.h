/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyunxue@gmail.com
# Date    :   2024/12/18 17:44:05
# Desc    :
########################################################################
*/
#pragma once

class Uncopyable {
protected:
    Uncopyable() = default;
    ~Uncopyable() = default;
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};  // class Uncopyable
