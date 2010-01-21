#!/bin/bash

ln -s /home/share/pole/tsp.repo/machinefile machinefile
scrun -group=pcc,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/eil51.tsp -b -m -t 
