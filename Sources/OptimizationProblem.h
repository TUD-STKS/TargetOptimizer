#pragma once
#include <dlib/matrix.h>
#include "Data.h"
#include "TamModelF0.h"


// parameter set defining an optimisation problem
struct ParameterSet
{
	double deltaSlope;
	double deltaOffset;
	double deltaTau;
	double lambda; // regularisation coefficient
	double weightSlope;
	double weightOffset;
	double weightTau;
	double meanSlope;
	double meanOffset;
	double meanTau;
};

// dlib linear algebra column vector for optimization tasks
typedef dlib::matrix<double, 0, 1> DlibVector;

// optimization problem for calculating pitch targets
class OptimizationProblem {
public:
	// constructors
	OptimizationProblem(const ParameterSet& parameters, const TimeSignal& originalF0, const BoundaryVector& bounds);

	// public member functions
	void setOptimum(const TargetVector& targets);

	ParameterSet getParameters() const;
	TimeSignal getModelF0() const;
	TargetVector getPitchTargets() const;
	Sample getOnset() const;
	double getCorrelationCoefficient() const;
	double getRootMeanSquareError() const;

	// operator called by optimizer
	double operator() (const DlibVector& arg) const;

private:
	// private member functions
	double costFunction(const TamModelF0& tamF0) const;
	static SampleTimes extractTimes(const TimeSignal& f0);
	static DlibVector signal2dlibVec(const TimeSignal& f0);

	// data members
	ParameterSet m_parameters;
	TimeSignal m_originalF0;
	BoundaryVector m_bounds;

	// store result
	TamModelF0 m_modelOptimalF0;
};