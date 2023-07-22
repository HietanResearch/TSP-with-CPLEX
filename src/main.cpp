#include "main.hpp"

int main(int argc, char *argv[]){
	if(argc < 4) {
		std::cerr << "ERROR: Input arguments" << std::endl;
		std::abort();
	}
	std::string parameter_filename = argv[1];
	std::string nodes_filename = argv[2];
	std::string routes_filename = argv[3];

	parameter::Parameter param(parameter_filename);

	int n = param.get<int>("n", 30);
	int size = param.get<int>("size", 100);

	Model model = Model(n, size);

	if(param.contains("seed")) {
		model.setSeed(param.get<int>("seed"));
	}

	model.generateRandom();

	Solver solver = Solver(model);

	bool res = solver.solve();

	if(res) {
		std::ofstream nodes_file(nodes_filename);
		nodes_file << model;

		std::ofstream routes_file(routes_filename);
		solver.printResult(routes_file);
	}

	return 0;
}
