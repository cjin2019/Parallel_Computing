#!/bin/bash
#mpirun -np $1                        -mca btl ^openib a.out
mpirun -np $1 -hostfile hosts.txt -mca btl ^openib a.out