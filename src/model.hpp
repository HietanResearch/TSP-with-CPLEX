#pragma once

#include "node.hpp"
#include <cmath>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

class Model {
	private:
		const int n;
		const int map_size;
		std::vector<node::Node> nodes;
		std::vector<std::vector<float>> c;
	public:
		Model(int arg_n, int arg_map_size);
		int getN();
		std::vector<std::vector<float>> getC();
		void generateModel();
		friend std::ofstream& operator<<(std::ofstream& out, const Model& g);
};
