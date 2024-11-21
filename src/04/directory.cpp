/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/20 18:54:27
# Desc    :
########################################################################
*/

#include "directory.h"

#include <numeric>

#include "cpp_utils/util.h"
#include "file_system.h"

namespace bad_case {
Directory temp_dir;
Directory::Directory() {
    disks_.resize(tfs.disk_nums());
    std::iota(disks_.begin(), disks_.end(), 0);
}

void Directory::print_disk_serial_num() {
    PRINT_CURRENT_FUNCTION_NAME;
    std::println("{}", disks_);
}
}  // namespace bad_case

namespace solution {
Directory::Directory() {
    disks_.resize(FileSystem::TheFileSystem().disk_nums());
    std::iota(disks_.begin(), disks_.end(), 0);
}

void Directory::print_disk_serial_num() {
    PRINT_CURRENT_FUNCTION_NAME;
    std::println("{}", disks_);
}
}  // namespace solution

namespace solution_2 {
Directory::Directory() {
    disks_.resize(tfs().disk_nums());
    std::iota(disks_.begin(), disks_.end(), 0);
}

void Directory::print_disk_serial_num() {
    PRINT_CURRENT_FUNCTION_NAME;
    std::println("{}", disks_);
}

Directory& temp_dir() {
    static Directory temp_dir;
    return temp_dir;
}
}  // namespace solution_2


