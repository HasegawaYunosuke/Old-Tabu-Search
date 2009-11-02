#! /bin/bash

gcc -c *.c
gcc -o a.out *.o -lm -pthread -g
rm *.o
ctags *.c
