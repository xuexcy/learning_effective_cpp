/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/20 18:47:29
# Desc    :
########################################################################
*/
#pragma once

#include <cstdint>

namespace bad_case {
class FileSystem {
public:
    uint32_t disk_nums() const {
        return 2;
    }
};  // class FileSystem
}  // namespace bad_case
extern bad_case::FileSystem tfs;

namespace solution {
class FileSystem {
public:
    uint32_t disk_nums() const {
        return 2;
    }
    static FileSystem& TheFileSystem() {
        static FileSystem tfs;
        return tfs;
    }
};  // class FileSystem
}  // namespace solution

namespace solution_2 {
class FileSystem {
public:
    uint32_t disk_nums() const {
        return 2;
    }
};  // class FileSystem
FileSystem& tfs();
}  // namespace solution_2




