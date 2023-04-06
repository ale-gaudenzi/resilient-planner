
#!/bin/bash


cd ~/planners/MyNDPlanner/translator-fond/

for i in {1..2}
do
	for j in {1..5}
	do
		python3.7 translate.py ~/planners/problems/grid/domain${i}k.pddl ~/planners/problems/grid/prob0${j}.pddl
		mv output.sas ~/planners/problems/grid/sas/p${j}k${i}.sas
		echo "created p${j}k${i}"
	done
done
