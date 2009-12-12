#!/bin/bash

ln -s /home/share/machinefile machinefile
scrun -group=machinefile,network=sctp ./a.out -f tsp_data/rat575.tsp -b -m -t 300
