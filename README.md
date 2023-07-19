# Traveling Salesman Problem with CPLEX

This program aims to solve the Traveling Salesman Problem (TSP), which is one of the combinatorial optimization problems, using the mathematical programming solver [IBM® CPLEX® Optimizer](https://www.ibm.com/products/ilog-cplex-optimization-studio/cplex-optimizer).

## Prequisites

* [IBM® CPLEX® Optimizer](https://www.ibm.com/products/ilog-cplex-optimization-studio/cplex-optimizer)
* C++20
* GCC (when using Linux)
* clang (when using Max OS X)
* R

## Compile and Run

* Get the code from GitHub
	* `git clone https://github.com/HietanResearch/TSP-with-CPLEX.git`
	* `git clone git@github.com:HietanResearch/TSP-with-CPLEX.git`
* Move to directory
	* `cd TSP-with-CPLEX`
* Run make
	* `make all`
* Run the executable and make graph
	* `make run_all`

## Makefile

* `all`: Compile all source files
* `execute`: Compile and execute
* `clean`: delete generated files

## Integer programming formulations

$$
	\large
	\begin{array}{lll}
		\mathrm{minimize}	& \displaystyle \sum_{u \in V} \sum_{v \in V, v \neq u} d_{uv} x_{uv}	& 																						\\
		\mathrm{s.t.}    	& \displaystyle \sum_{u \in V, u \neq v} x_{uv} = 1,									& v \in V,																		\\
											& \displaystyle \sum_{u \in V, u \neq v} x_{vu} = 1,									& v \in V,																		\\
											& y_u - y_v + \left| V \right| x_{uv} \leq \left| V \right| - 1,			& u,v \in V \backslash \\{ s \\} , u \neq v,	\\
											& x_{uv} \in \{ 0, 1 \} ,																							& u,v \in V, u \neq v,												\\
											& y_s = 0, 																														& 																						\\
											& 1 \leq y_v \leq \left| V \right| - 1,																						& v \in V \backslash \\{ s \\}.							
	\end{array}
$$

## Structure

* `main.cpp`
* `model.cpp`
* `data.cpp`
* `node.cpp`
* `solver.cpp`
