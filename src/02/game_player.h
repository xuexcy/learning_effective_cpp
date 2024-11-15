/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyun@gmail.com
# Date    :   2024/11/15 15:26:29
# Desc    :
########################################################################
*/
#pragma once

class GamePlayer {
private:
    // 声明式
    static const int NumTurns;

    // 声明式赋初始值，只支持整数类(int char bool)
    static const int N1 = 5;
    int scores[N1];

    // 编译器可能不支持声明式赋初始值，可以通过 enum 来解决数组来解决此问题
    enum { N2 = 5};
    int scores_2[N2];
};

