# Multiple Traveling Salesman Problem with CPLEX

## Prequisites

* CPLEX
* C++
* GCC (when using Linux)
* clang (when using Max OS X)
* R
* Rscript

## Compile and Run

* Get the code from GitHub
	* `git clone https://github.com/HietanResearch/mTSP.git`
	* `git clone git@github.com:HietanResearch/mTSP.git`
* Move to a build directory
	* `cd mTSP/build_x86-64_linux` (when using Linux)
	* `cd mTSP/build_arm64_osx` (when using Max OS X)
* Run make
	* `make all`
* Run the executable
	* `../bin/main.out`

## Makefile

* `all`: Compile all source files
* `execute`: Compile and execute
* `clean`: delete generated files

## Structure

* `main.cpp`
* `model.cpp`
* `data.cpp`
* `node.cpp`
* `solver.cpp`
