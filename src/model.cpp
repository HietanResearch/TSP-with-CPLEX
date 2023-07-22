#include "model.hpp"

Model::Model(int arg_n, int arg_size) : n{arg_n}, size{arg_size} {
	c = std::vector<std::vector<float>>(n, std::vector<float>(n));
	generateModel();
}

int Model::getN() const {
	return n;
}

std::vector<std::vector<float>> Model::getC() const {
	return c;
}

void Model::generateModel() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0, size);

	nodes = std::vector<node::Node>(n);

	for(int i = 0; i < n; i++) {
		int x = dist(mt);
		int y = dist(mt);
		nodes.at(i) = node::Node(x, y, i);
	}

	for(int y = 0; y < n; y++) {
		for(int x = 0; x <= y; x++) {
			if(y == x) c.at(y).at(x) = FLT_MAX;
			else {
				float d = distance(nodes.at(y), nodes.at(x));
				c.at(y).at(x) = d;
				c.at(x).at(y) = d;
			}
		}
	}
}

std::ofstream& operator << (std::ofstream& out, const Model& g) {
	for(const node::Node& n: g.nodes) out << n;
	return out;
}
