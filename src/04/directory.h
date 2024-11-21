/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/20 18:50:09
# Desc    :
########################################################################
*/
#pragma once

#include <cstdint>
#include <vector>

namespace bad_case {
class Directory {
public:
    static Directory& TempDir() {
        static Directory temp_dir;
        return temp_dir;
    }
public:
    Directory();
    void print_disk_serial_num();
private:
    std::vector<uint8_t> disks_;
};  // class Directory

extern Directory temp_dir;

}  // namespace bad_case

namespace solution {
class Directory {
public:
    static Directory& TempDir() {
        static Directory temp_dir;
        return temp_dir;
    }
public:
    Directory();
    void print_disk_serial_num();
private:
    std::vector<uint8_t> disks_;
};  // class Directory

}  // namespace solution

namespace solution_2 {
class Directory {
public:
    Directory();
    void print_disk_serial_num();
private:
    std::vector<uint8_t> disks_;
};  // class Directory
Directory& temp_dir();
}  // namespace solution_2



