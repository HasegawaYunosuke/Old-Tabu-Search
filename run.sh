#!/bin/bash

ln -s  /home/share/pole/tsp.repo machinefile
<<<<<<< HEAD
scrun -group=pcc,network=sctp -nodes=8x1 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 600
=======
scrun -group=pcc,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/eil51.tsp -l -m -t 120
>>>>>>> e45983467eec3fc51907e26acc2ab8d181225091
