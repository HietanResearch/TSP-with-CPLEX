#include "solver.hpp"

namespace solver {
	Solver::Solver(model::Model arg_my_model) : my_model{arg_my_model} {}

	bool Solver::solve(){
		int n = my_model.getN();
		std::vector<std::vector<float>> c = my_model.getC();

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
			u[i] = IloNumVar(env, 1, n - 1, IloNumVar::Int, name.str().c_str());
			name.str("");
		}

		IloExpr expr(env);

		// Objective Function
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(i == j) continue;
				expr += c.at(i).at(j) * x[i][j];
			}
		}
		IloObjective obj(env, expr, IloObjective::Minimize);

		model.add(obj);
		expr.clear();

		// Constraints (1)
		IloRangeArray const1(env, n);
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				if(i == j) continue;
				expr += x[i][j];
			}
			name << "Constrains(1)(i_" << i << ")";
			const1[i] = IloRange(env, 1, expr, 1, name.str().c_str());
			name.str("");
			expr.clear();
		}

		model.add(const1);

		// Constraints (2)
		IloRangeArray const2(env, n);
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

		// Constraints (3)
		IloArray<IloRangeArray> const3(env, n);
		for(int i = 1; i < n; i++){
			const3[i] = IloRangeArray(env, n);

			for(int j = 1; j < n; j++){
				if(i == j) continue;

				expr += u[i] - u[j] + n * x[i][j];

				name << "Constraints(4)(i_" << i << ",j_" << j << ")";
				const3[i][j] = IloRange(env, -IloInfinity, expr, n - 1, name.str().c_str());

				expr.clear();
				name.str("");
			}
			model.add(const3[i]);
		}

		expr.end();
		
		// Solve
		IloCplex cplex(model);

		bool solved = false;

		try {
			solved = cplex.solve();
		}
		catch(const IloException& error){
			std::cerr << std::endl << "CPLEX Raised an exception" << std::endl;
			std::cerr << error << std::endl;
			env.end();
			throw;
		}

		if(solved){
			std::cout << std::endl << "CPLEX Solved!" << std::endl;
			std::cout << "\tStatus: " << cplex.getStatus() << std::endl;;
			std::cout << "\tObjective value: " << cplex.getObjValue() << std::endl << std::endl;

			result = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
			for(int i = 0; i < n; i++){
				for(int j = 0; j < n; j++){
					if(i == j) {
						result.at(i).at(j) = 0;
						continue;
					}
					result.at(i).at(j) = cplex.getValue(x[i][j]);
				}
			}
		}
		else {
			std::cerr << std::endl << "CPLEX Error!" << std::endl;
			std::cerr << "\tStatus: " << cplex.getStatus() << std::endl;
			std::cerr << "\tSolve status: " << cplex.getCplexStatus() << std::endl << std::endl;
		}

		env.end();

		return solved;
	}

	void Solver::printResult(std::ofstream& file){
		for(vector<int> r : result){
			for(int i = 0; i < (int)r.size(); i++){
				file << r.at(i);
				if(i < (int)r.size() - 1) file << ", ";
			}
			file << std::endl;
		}
	}
}
