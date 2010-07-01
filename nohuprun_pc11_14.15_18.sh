#!/bin/bash

#ln -s  /home/share/pole/tsp.repo machinefile
scrun -group=pc11_14machinefile,network=sctp -nodes=4x2 ./nosamegroup.exe -f tsp_data/pr1002.tsp -l -m -p -t 7200 &
sleep 5
scrun -group=pc15_18machinefile,network=sctp -nodes=4x2 ./samegroup.exe -f tsp_data/pr1002.tsp -l -m -p -t 7200
