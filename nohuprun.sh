#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi3.v1d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi3.v2d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi1.v3d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi2.v3d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi3.v3d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi1.v4d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi2.v4d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi3.v4d.exe -f tsp_data/rat575.tsp -l -m -p -t 1800
