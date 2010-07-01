#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=machinefile,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/pr1002.tsp -l -m -p -t 7200
