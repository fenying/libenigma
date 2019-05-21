#!/bin/bash

source ~/.bash_profile
SCRIPT_ROOT=$(cd `dirname $0`; pwd)

gcc -O3 -o $SCRIPT_ROOT/enigma.exe \
    $SCRIPT_ROOT/src/encrypt.c \
    $SCRIPT_ROOT/src/lib/src/*.c
