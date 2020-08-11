#pragma once
#include "OptimizationProblem.h"


// solver for an optimization problem utilizing BOBYQA algorithm
class BobyqaOptimizer {
public:
	// constructors
	BobyqaOptimizer() { };//srand(3);};//srand(time(NULL)); };

	// public member functions
	void optimize(OptimizationProblem& op) const;
	//void optimize_targets_and_boundaries(OptimizationProblem& op) const;

private:
	// private member functions
	static double getRandomValue(const double min, const double max);
};