#!/bin/bash

<<<<<<< HEAD:run.sh
ln -s /home/share/pole/tsp.repo/machinefile machinefile
scrun -group=pcc,network=sctp -nodes=4x2 ./mpi.exe -f tsp_data/eil51.tsp -b -m -t 
=======
ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=pcc,network=sctp -nodes=8x2 ./mpi.exe -f tsp_data/eil51.tsp -l -m -t 120
>>>>>>> c423fbeb4455c097ad8f34d6af7c64aa1ff8c4c6:run.sh
