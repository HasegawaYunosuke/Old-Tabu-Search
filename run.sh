#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
if [ $1 == 1 ]
then
scrun -group=machinefile,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp  -l -m -t 600
fi
if [ $1 == 2 ]
then
scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp -l -m -t 30
fi
if [ $1 == 3 ]
then
scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp -b -m -t 30
fi
if [ $1 == 4 ]
then
scrun -group=pcc,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp -b -m -t 30
fi
