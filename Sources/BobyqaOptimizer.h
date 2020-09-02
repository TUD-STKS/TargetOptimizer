#pragma once
#include "OptimizationProblem.h"
#include <string>

struct OptimizerOptions
{
	int maxIterations{ 100 };
	bool useEarlyStopping{ true };
	double epsilon{ 0.01 };
	int patience{ 10 };
	int maxCostEvaluations{ 100000 };
	double rhoEnd{ 0.000001 };
};

// solver for an optimization problem utilizing BOBYQA algorithm
class BobyqaOptimizer {
public:
	// constructors
	BobyqaOptimizer() = default;

	// public member functions
	void optimize(OptimizationProblem& op, OptimizerOptions optOpt ) const;
	//void optimize_targets_and_boundaries(OptimizationProblem& op) const;

private:
	// private member functions
	static double getRandomValue(const double min, const double max);
};