#pragma once
#include "OptimizationProblem.h"
#include <string>

#ifdef USE_WXWIDGETS
#include <wx/progdlg.h>
#endif

struct OptimizerOptions
{
	int maxIterations{ 1000 };
	bool useEarlyStopping{ false };
	double epsilon{ 0.1 };  // Minimum relative improvement to update the optimum if early stopping is used
	int patience{ 15 };  // If after this many iterations the minimum has not improved, the optimization stops. The default value is updated when the number of targets to be found is known according to the following formula: patience = 15 * numberOfTargets
	int maxCostEvaluations{ 100000 };
	double rhoEnd{ 0.001 };
};

// solver for an optimization problem utilizing BOBYQA algorithm
class BobyqaOptimizer {
public:
	// constructors
	BobyqaOptimizer() = default;

	// public member functions
	void optimize(OptimizationProblem& op, OptimizerOptions optOpt);
#ifdef USE_WXWIDGETS
	// Decorator design pattern: When the GUI is used, update an optional progress dialog after every iteration
	void optimize(OptimizationProblem& op, OptimizerOptions optOpt, wxGenericProgressDialog* waitbar);
#endif
	//void optimize_targets_and_boundaries(OptimizationProblem& op) const;

private:
	// private member functions
	static double getRandomValue(const double min, const double max);
#ifdef USE_WXWIDGETS
	wxGenericProgressDialog* waitbar_{ nullptr };
#endif
};