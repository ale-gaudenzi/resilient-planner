# Resilient Planner

## Build
```
cd src/
./build_all
```

## Examples
```
/src/resplanner /problems/simpletravel/domain.pddl /problems/simpletravel/pfile1 --resilient 1
/src/resplanner /problems/simpletravel/domain.pddl /problems/satellite/5 --resilient 2
```

## Usage
```
./src/resplanner <domain> <problem> OPTIONS

--resilient K
    Number of maximum faults allowed

--verbose 1/0
    Verbose mode

--max-iterations N
    Limit the iterations of the algorithm

--plan-to-file 1/0
    Dump the resilient plan to a file

--dump-branches 1/0
    Print branches of the replan to a JSON file

--dump-resilient-policy 1/0
    Print resilient policy to a JSON file

--dump-resilient-nodes 1/0
    Print all resilient nodes found

--print-memory-replan-progression 1/0
    Print memory usage for every replan
```

