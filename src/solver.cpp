#include "solver.hpp"

#define OK(n) std::cout << "OK" << n << std::endl

Solver::Solver(Graph arg_graph) : graph{arg_graph} {}

bool Solver::solve(){
	int n = graph.getN();
	std::vector<std::vector<float>> c = graph.getC();
	int m = graph.m;
	int L = graph.L;
	int K = graph.K;

	IloEnv env;
	IloModel model(env);

	std::stringstream name;

	// Variables x
	IloArray<IloNumVarArray> x(env, n);
	for(int i = 0; i < n; i++){
		x[i] = IloNumVarArray(env, n);
		for(int j = 0; j < n; j++){
			name << "x(" << i << ")(" << j << ")";
			x[i][j] = IloNumVar(env, 0, 1, IloNumVar::Bool, name.str().c_str());
			name.str("");
		}
	}

	// Variables u
	IloNumVarArray u(env, n);
	u[0] = IloNumVar(env, 0, 0, IloNumVar::Int, "u(0)");
	for(int i = 1; i < n; i++){
		name << "u(" << i << ")";
		u[i] = IloNumVar(env, 1, L - 1, IloNumVar::Int, name.str().c_str());
		name.str("");
	}

	IloExpr expr(env);

	// Objective Function (1)
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			expr += c.at(i).at(j) * x[i][j];
		}
	}
	IloObjective obj(env, expr, IloObjective::Minimize);

	model.add(obj);
	expr.clear();

	// Constraints (2)
	for(int j = 1; j < n; j++){
		expr += x[0][j];
	}
	IloRange const2(env, m, expr, m, "Constraints(2)");

	model.add(const2);
	expr.clear();

	// Constraints (3)
	for(int i = 1; i < n; i++){
		expr += x[i][0];
	}
	IloRange const3(env, m, expr, m, "Constraints(3)");

	model.add(const3);
	expr.clear();

	// Constraints (4)
	IloRangeArray const4(env, n - 1);
	for(int j = 1; j < n; j++){
		for(int i = 0; i < n; i++){
			expr += x[i][j];
		}

		name << "Constraints(4)(j_" << j << ")";
		const4[j - 1] = IloRange(env, 1, expr, 1, name.str().c_str());
		expr.clear();
		name.str("");
	}

	model.add(const4);

	// Constraints (5)
	IloRangeArray const5(env, n - 1);
	for(int i = 1; i < n; i++){
		for(int j = 0; j < n; j++){
			expr += x[i][j];
		}

		name << "Constraints(5)(i_" << i << ")";
		const5[i - 1] = IloRange(env, 1, expr, 1, name.str().c_str());
		expr.clear();
		name.str("");
	}

	model.add(const5);

	// Constraints (6)
	IloRangeArray const6(env, n - 1);
	for(int i = 1; i < n; i++){
		expr += u[i] + (L - 2) * x[0][i] - x[i][0];

		name << "Constraints(6)(i_" << i << ")";
		const6[i - 1] = IloRange(env, -IloInfinity, expr, L - 1, name.str().c_str());

		expr.clear();
		name.str("");
	}

	model.add(const6);

	// Constraints (7)
	IloRangeArray const7(env, n - 1);
	for(int i = 1; i < n; i++){
		expr += u[i] + x[0][i] + (2 - K) * x[i][0];

		name << "Constraints(7)(i_" << i << ")";
		const7[i - 1] = IloRange(env, 2, expr, IloInfinity, name.str().c_str());

		expr.clear();
		name.str("");
	}

	model.add(const7);

	// Constraints (8)
	IloRangeArray const8(env, n - 1);
	for(int i = 1; i < n; i++){
		expr += x[0][i] + x[i][0];

		name << "Constraints(8)(i_" << i << ")";
		const8[i - 1] = IloRange(env, -IloInfinity, expr, 1, name.str().c_str());

		expr.clear();
		name.str("");
	}

	model.add(const8);

	// Constraints (9)
	IloArray<IloRangeArray> const9(env, n);
	for(int i = 1; i < n; i++){
		const9[i] = IloRangeArray(env, n);

		for(int j = 1; j < n; j++){
			if(i == j) continue;

			expr += u[i] - u[j] + L * x[i][j] + (L - 2) * x[j][i];

			name << "Constraints(9)(i_" << i << ",j_" << j << ")";
			const9[i][j] = IloRange(env, -IloInfinity, expr, L - 1, name.str().c_str());

			expr.clear();
			name.str("");
		}
		model.add(const9[i]);
	}

	expr.end();
	
	// Solve
	IloCplex cplex(model);

	cplex.exportModel("./out/model.lp");

	bool solved = false;

	try {
		solved = cplex.solve();
	} catch(const IloException& e){
		std::cerr << "\n\nCPLEX Raised an exception:\n";
		std::cerr << e << "\n";
		env.end();
		throw;
	}

	if(solved){
		std::cout << "\n\nCPLEX Solved!\n";
		std::cout << "\tStatus: " << cplex.getStatus() << "\n";
		std::cout << "\tObjective value: " << cplex.getObjValue() << "\n";
		res = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(cplex.getValue(x[i][j]) > 0.5) res.at(i).at(j) = 1;
			}
		}
	}
	else {
		std::cerr << "\n\nCPLEX Error!\n";
		std::cerr << "\tStatus: " << cplex.getStatus() << "\n";
		std::cerr << "\tSolve status: " << cplex.getCplexStatus() << "\n";
	}

	env.end();

	return solved;
}

void Solver::printResult(std::ofstream& file){
	for(vector<int> r : res){
		for(int i = 0; i < r.size(); i++){
			file << r.at(i);
			if(i < r.size() - 1) file << ", ";
		}
		file << std::endl;
	}
}
