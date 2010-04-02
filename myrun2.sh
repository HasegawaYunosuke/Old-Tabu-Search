#!/bin/bash
j=0
for j in 2 4 6 8; do
echo $j
scrun -group=machinefile2,network=sctp -nodes=4x2 ./nompi2.exe -f tsp_data/rat575.tsp -l -m -t 1800
sleep 10
done
