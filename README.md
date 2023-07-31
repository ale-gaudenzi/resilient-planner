# Resilient Planner

## Build
```
./src/build_all
```

## Examples
```
/src/resplanner /benchmarks/IPC/zenotravel/domain.pddl /benchmarks/IPC/zenotravel/pfile1 --resilient 1 --dump-resilient-policy 1
/src/resplanner /benchmarks/Res/rockect/domain.pddl /benchmarks/Res/rocket/pfilep1.pddl --resilient 2 --dump-resilient-policy 1 --dump-resilient-nodes 1
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

## Documentation
```
The full documentation can be found [here](html/index.html).
```
