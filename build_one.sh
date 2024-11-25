#!/bin/bash
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/25 15:14:20
# Desc    :
########################################################################

# set -x
CUR_DIR=$(cd `dirname $0`; pwd)
cd ${CUR_DIR}

source ${CUR_DIR}/env_variable.sh


main=$default_execute_file
if [ $# -gt 0 ]; then
    main=$1
fi

cd ${build_dir}
make $main








cd -



