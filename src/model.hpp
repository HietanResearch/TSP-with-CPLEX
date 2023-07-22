#pragma once

#include "node.hpp"
#include <cfloat>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

class Model {
	private:
		const int n;
		const int size;
		std::vector<node::Node> nodes;
		std::vector<std::vector<float>> c;
	public:
		Model(int arg_n, int arg_size);
		int getN() const;
		std::vector<std::vector<float>> getC() const;
		void generateModel();
		friend std::ofstream& operator<<(std::ofstream& out, const Model& g);
};
