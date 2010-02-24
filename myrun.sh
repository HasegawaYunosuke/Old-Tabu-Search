#!/bin/bash
j=0
for j in 1 2 3 4 5; do
scrun -group=machinefile,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/rat575.tsp -l -m -t 1800
sleep 10
done
