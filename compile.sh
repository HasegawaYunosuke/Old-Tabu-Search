#! /bin/bash

gcc -c main.c show.c argument.c error.c finalize.c hasegawa.c history.c initial_path.c initialize.c mallocer.c mannerism.c math.c parameter.c pole.c read_data.c search.c tabu.c terminate.c timer.c tozaki.c visualize.c
gcc -o a.out main.o show.o argument.o error.o finalize.o hasegawa.o history.o initial_path.o initialize.o mallocer.o mannerism.o math.o parameter.o pole.o read_data.o search.o tabu.o terminate.o timer.o tozaki.o visualize.o -lm -pthread -g
rm *.o
ctags *.c
