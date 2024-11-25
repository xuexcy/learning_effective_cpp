#!/bin/bash
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/25 14:41:26
# Desc    :
########################################################################

# set -x
CUR_DIR=$(cd `dirname $0`; pwd)
cd ${CUR_DIR}

source ${CUR_DIR}/env_variable.sh

cd $build_output_bin_dir

if [ $# -gt 0 ]; then
    ./$1
else
    ./$default_execute_file
fi






cd -



