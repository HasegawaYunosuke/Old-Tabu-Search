#!/bin/bash

ln -s /home/share/machinefile machinefile
#scrun -group=pcc,network=sctp -nodes=2x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 30 -v 2
scrun -group=pcc,network=sctp -nodes=8x1 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 30
