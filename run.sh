#!/bin/bash

ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=pcc,network=sctp -nodes=8x1 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 600
