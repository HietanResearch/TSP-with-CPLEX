#pragma once

#include "model.hpp"
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <string>

ILOSTLBEGIN

class Solver {
	public:
		Model mod;
		std::vector<std::vector<int>> res;
		Solver(Model arg_Model);
		bool solve();
		void printResult(std::ofstream& file);
};
