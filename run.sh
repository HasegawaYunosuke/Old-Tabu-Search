#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
if [ $1 == 1 ]
then
scrun -group=machinefile,network=sctp -nodes=4x2 ./nompi.exe -f tsp_data/lin318.tsp  -l -t 1800
fi
if [ $1 == 2 ]
then
scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp -l -m -t 30
fi
if [ $1 == 3 ]
then
scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 1800
fi
if [ $1 == 4 ]
then
#scrun -group=pcc,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp -b -m -t 30
scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/eil51.tsp -b -t 60
fi
if [ $1 == 5 ]
then
scrun -group=machinefile,network=sctp -nodes=4x2 ./nompi.exe -f tsp_data/rat575.tsp -b -t 30
fi
if [ $1 == 6 ]
then
#scrun -group=machinefile,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 60
scrun -group=pcc,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 1800
fi
