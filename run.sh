#!/bin/bash

ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=pcc,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/eil51.tsp -b -m -t 300
