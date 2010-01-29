#!/bin/bash

scrun -group=machinefile2,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 1800
