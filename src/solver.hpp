#pragma once

#include "graph.hpp"
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>

ILOSTLBEGIN

class Solver {
	public:
		Graph graph;
		std::vector<std::vector<int>> res;
		Solver(Graph arg_Graph);
		bool solve();
		void printResult(std::ofstream& file);
};
