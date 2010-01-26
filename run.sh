#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
if [ $1 > 0 ]
then
scrun -group=machinefile,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp  -l -m -t 600
else
scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/lin318.tsp -l -m -t 600
fi
