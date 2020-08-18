#pragma once
#include "OptimizationProblem.h"
#include <string>

struct OptimizerOptions
{
	int maxIterations{ 30 };
	bool useEarlyStopping{ false };
	double epsilon{ 0.01 };
	int patience{ 5 };
	int maxCostEvaluations{ 100000 };
	double rhoEnd{ 0.000001 };
};

// solver for an optimization problem utilizing BOBYQA algorithm
class BobyqaOptimizer {
public:
	// constructors
	BobyqaOptimizer() { srand(time(NULL)); };//srand(3);};//srand(time(NULL)); };

	// public member functions
	void optimize(OptimizationProblem& op, OptimizerOptions optOpt, std::string LOG_PATH = "" ) const;
	//void optimize_targets_and_boundaries(OptimizationProblem& op) const;

private:
	// private member functions
	static double getRandomValue(const double min, const double max);
};