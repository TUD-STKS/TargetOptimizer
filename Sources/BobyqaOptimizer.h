#pragma once
#include "OptimizationProblem.h"
#include <string>

#ifdef USE_WXWIDGETS
#include <wx/progdlg.h>
#endif


struct OptimizerOptions
{
	int maxIterations{ 100 };
	bool useEarlyStopping{ true };
	double epsilon{ 0.01 };
	int patience{ 10 };
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