#!/bin/bash

ln -s /home/share/machinefile machinefile
scrun -group=machinefile,network=sctp ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 3