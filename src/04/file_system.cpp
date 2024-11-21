/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/20 18:49:52
# Desc    :
########################################################################
*/

#include "file_system.h"

bad_case::FileSystem tfs;

namespace solution_2 {
FileSystem& tfs() {
    static FileSystem tfs;
    return tfs;
}

}  // namespace solution

