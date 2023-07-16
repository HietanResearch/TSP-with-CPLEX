#include "main.hpp"

int main(int argc, char *argv[]){
	if(argc < 2) {
		std::cerr << "ERROR: Input arguments" << std::endl;
	}
	std::string filename = argv[1];

	parameter::Parameter param(filename);

	int n = param.get<int>("n", 30);
	int size = param.get<int>("size", 100);

	Model model = Model(n, size);

	Solver solver = Solver(model);

	bool res = solver.solve();

	if(res) {
		std::ofstream nodes_file("out/nodes.csv");
		nodes_file << model;

		std::ofstream routes_file("out/routes.csv");
		solver.printResult(routes_file);
	}

	return 0;
}
