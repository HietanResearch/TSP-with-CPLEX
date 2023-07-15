#pragma once

#include "data.hpp"
#include "graph.hpp"
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>

ILOSTLBEGIN

class Solver {
	public:
		Graph graph;
		Data data;
		std::vector<std::vector<int>> res;
		Solver(Graph arg_Graph, Data arg_data);
		bool solve();
		void printResult(std::ofstream& file);
};
