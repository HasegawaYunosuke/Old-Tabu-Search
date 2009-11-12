#! /bin/bash

gcc-4.3 -c *.c
gcc-4.3 -o a.out *.o -lm -pthread -g
rm *.o
ctags *.c
