#!/bin/bash

ln -s /home/share/machinefile machinefile
<<<<<<< HEAD:run.sh
scrun -group=machinefile,network=sctp ./mpi.exe -f tsp_data/rat575.tsp -l -m -t 300
=======
#scrun -group=pcc,network=sctp -nodes=2x2 ./mpi.exe -f tsp_data/rat575.tsp -b -m -t 30 -v 2
scrun -group=pcc,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/eil51.tsp -b -m -t 120
>>>>>>> ed622096f17959621fd1e7c8cdec8db53796d6c0:run.sh
