#!/bin/bash

ln -s /home/share/machinefile machinefile
<<<<<<< HEAD:run.sh
scrun -group=machinefile,network=sctp -nodes=2x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 30 -v 2
#scrun -group=machinefile,network=sctp ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 3
=======
#scrun -group=machinefile,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 10
scrun -group=machinefile,network=sctp ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 3
>>>>>>> 8d3422206ff7b25806f3991aa024b49b82aabf2c:run.sh
