#pragma once

#include "node.hpp"
#include <cmath>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

class Graph {
	private:
		const int n;
		const int map_size;
		std::vector<Node> nodes;
		std::vector<std::vector<float>> c;
	public:
		Graph(int arg_n, int arg_map_size);
		int getN();
		std::vector<std::vector<float>> getC();
		void generateGraph();
		friend std::ofstream& operator<<(std::ofstream& out, const Graph& g);
};
