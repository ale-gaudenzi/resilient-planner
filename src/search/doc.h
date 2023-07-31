/*! \mainpage Resilient Planner
* \section brief_sec Brief Description
* This is the documentation of the Resilient Planner, a planner for resolving automated planning problems assuming a limited number of failures of the deterministic action.
* \subsection Usage
* The planner can be used as follows:
* \code ./src/resplanner <domain> <problem> [options]\endcode
* In the benchmarks directory there are some examples of domains and problems.
* \subsection Examples
* \code ./src/resplanner domain.pddl problem.pddl --resilient 1 --plan-to-file 1 --dump-branches 1 --dump-resilient-policy 1 --dump-resilient-nodes 1 \endcode
* \subsection Building
* To build the planner, run the following command:
* \code ./src/build_all
* \subsection Options
* The following options are available:
* \li \c --resilient K: Maximum number of failures allowed
* \li \c --verbose 1/0: Verbose mode
* \li \c --max-iterations N: Limit the iterations of the algorithm
* \li \c --plan-to-file 1/0: Dump the resilient plan to a file
* \li \c --dump-branches 1/0: Print branches of the replan to a JSON file
* \li \c --dump-resilient-policy 1/0: Print resilient policy to a JSON file
* \li \c --dump-resilient-nodes 1/0: Print all resilient nodes found
* \li \c --print-memory-replan-progression 1/0: Print memory usage for every replan
 */