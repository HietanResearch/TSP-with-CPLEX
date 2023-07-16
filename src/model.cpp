#include "model.hpp"

Model::Model(int arg_n, int arg_map_size, int arg_m, int arg_K, int arg_L) : n{arg_n}, map_size{arg_map_size}, m{arg_m}, K{arg_K}, L{arg_L} {
	c = std::vector<std::vector<float>>(n, std::vector<float>(n));
	generateModel();
}

int Model::getN(){
	return n;
}

std::vector<std::vector<float>> Model::getC(){
	return c;
}

void Model::generateModel() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0, map_size);

	nodes = std::vector<Node>(n);

	for(int i = 0; i < n; i++){
		int x = dist(mt);
		int y = dist(mt);
		nodes.at(i) = Node(x, y, i);
	}

	for(int i = 0; i < n; i++){
		for(int j = 0; j <= i; j++){
			if(i == j) c.at(i).at(j) = FLT_MAX;
			else {
				float d = distance(nodes.at(i), nodes.at(j));
				c.at(i).at(j) = d;
				c.at(j).at(i) = d;
			}
		}
	}
}

std::ofstream& operator<<(std::ofstream& out, const Model& g){
	for(const Node& n: g.nodes){
		out << n.x << ',' << n.y << std::endl;
	}
	return out;
}
