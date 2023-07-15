#include "main.hpp"

int main(int argc, char *argv[]){
	Graph graph = Graph(30, 100);
	Data data = Data(4, 15, 5);

	Solver solver = Solver(graph, data);

	bool res = solver.solve();

	if(res) {
		std::ofstream nodes_file("out/nodes.csv");
		nodes_file << graph;

		std::ofstream routes_file("out/routes.csv");
		solver.printResult(routes_file);
	}

	return 0;
}
