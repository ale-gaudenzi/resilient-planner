
#!/bin/bash


cd ~/planners/MyNDPlanner/translator-fond/

for i in {1..2}
do
	for j in {1..9}
	do
		python3.7 translate.py ~/planners/problems/satellite/domain${i}k.pddl ~/planners/problems/satellite/p${j}
		mv output.sas ~/planners/problems/satellite/sas/p${j}k${i}.sas
		echo "created p${j}k${i}"
	done
done
