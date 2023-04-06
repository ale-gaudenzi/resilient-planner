#!/bin/bash


cd ~/planners/MyNDPlanner/translator-fond/

for i in {1..2}
do
	for j in {1..9}
	do
		python3.7 translate.py ~/planners/problems/storage/domain${i}k.pddl ~/planners/problems/storage/p${j}
		mv output.sas ~/planners/problems/storage/sas/p${j}k${i}.sas
		echo "created p${j}k${i}"
	done
done
