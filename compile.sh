#! /bin/bash

gcc -c *.c
gcc -o a.out *.o -lm -pthread
rm *.o
