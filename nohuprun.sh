#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/rat575.tsp -l -m -p -t 3600
sleep 5
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/rat575.tsp -l -m -p -t 3600
#sleep 5
#scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/rat575.tsp -l -m -p -t 3600
