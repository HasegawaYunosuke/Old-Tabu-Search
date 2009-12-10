#! /bin/bash

echo "header.hの#define MPIMODEのコメントアウト外してますか？3秒sleepします。"
echo "+++以下はheader.hの先頭行+++"
head -n 1 header.h
echo "+++以上はheader.hの先頭行+++"
sleep 3
mpicc -c main.c show.c parallel.c argument.c error.c mpi_initialize.c mpi_finalize.c hasegawa.c history.c initial_path.c mallocer.c mannerism.c math.c mpi_parameter.c pole.c read_data.c search.c tabu.c terminate.c timer.c tozaki.c visualize.c
mpicc -o a.out main.o show.o parallel.o argument.o error.o mpi_initialize.o mpi_finalize.o hasegawa.o history.o initial_path.o mallocer.o mannerism.o math.o mpi_parameter.o pole.o read_data.o search.o tabu.o terminate.o timer.o tozaki.o visualize.o -lm -pthread -g
rm *.o
ctags *.c
ln -s /home/share/machinefile machinefile
