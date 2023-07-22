#include "solver.hpp"

Solver::Solver(model::Model arg_mod) : mod{arg_mod} {}

bool Solver::solve(){
	int n = mod.getN();
	std::vector<std::vector<float>> c = mod.getC();

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
		u[i] = IloNumVar(env, 1, n, IloNumVar::Int, name.str().c_str());
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
	IloRangeArray const2(env, n);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if(i == j) continue;
			expr += x[i][j];
		}
		name << "Constrains(2)(i_" << i << ")";
		const2[i] = IloRange(env, 1, expr, 1, name.str().c_str());
		name.str("");
		expr.clear();
	}

	model.add(const2);

	// Constraints (3)
	IloRangeArray const3(env, n);
	for(int j = 0; j < n; j++) {
		for(int i = 0; i < n; i++) {
			if(i == j) continue;
			expr += x[i][j];
		}
		name << "Constrains(2)(j_" << j << ")";
		const2[j] = IloRange(env, 1, expr, 1, name.str().c_str());
		name.str("");
		expr.clear();
	}

	model.add(const2);
	expr.clear();

	// Constraints (4)
	IloArray<IloRangeArray> const4(env, n);
	for(int i = 1; i < n; i++){
		const4[i] = IloRangeArray(env, n);

		for(int j = 1; j < n; j++){
			if(i == j) continue;

			expr += u[i] - u[j] + n * x[i][j];

			name << "Constraints(4)(i_" << i << ",j_" << j << ")";
			const4[i][j] = IloRange(env, -IloInfinity, expr, n - 1, name.str().c_str());

			expr.clear();
			name.str("");
		}
		model.add(const4[i]);
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
		for(int i = 0; i < (int)r.size(); i++){
			file << r.at(i);
			if(i < (int)r.size() - 1) file << ", ";
		}
		file << std::endl;
	}
}
