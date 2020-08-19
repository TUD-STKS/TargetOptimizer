#pragma once
#include <dlib/matrix.h>
#include "Data.h"
#include "TamModelF0.h"

#include <tuple>


struct RegularizationParameters
{
	double lambda{ 0.01 };
	double weightSlope{ 1.0 };
	double weightOffset{ 0.5 };
	double weightTau{ 0.1 };
};

struct SearchSpaceParameters
{
	double deltaSlope{ 50.0 };
	double deltaOffset{ 20.0 };
	double deltaTau{ 5.0 };
	double deltaBoundary{ 100.0 };
	int    initBounds{ 0 };
	bool   optimizeBoundaries{ deltaBoundary > 0 };
	int    numberOptVar{ optimizeBoundaries ? 4 : 3 };
	double meanSlope{ 0.0 };
	double meanOffset{ 0.0 };
	double meanTau{ 20.0 };
};

// parameter set defining an optimisation problem
struct ParameterSet
{
	RegularizationParameters regularizationParameters;
	SearchSpaceParameters searchSpaceParameters;
};

// dlib linear algebra column vector for optimization tasks
typedef dlib::matrix<double, 0, 1> DlibVector;

// optimization problem for calculating pitch targets
class OptimizationProblem {
public:
	// constructors
	OptimizationProblem(const ParameterSet& parameters, const TimeSignal& originalF0, const BoundaryVector& bounds);

	// public member functions
	void setOptimum(const BoundaryVector& boundaries, const TargetVector& targets);
	void setBoundaries(const BoundaryVector& boundaries);

	ParameterSet getParameters() const;
	TimeSignal getModelF0() const;
	TargetVector getPitchTargets() const;
	BoundaryVector getBoundaries() const;
	Sample getOnset() const;
	double getOriginalF0_Onset() const;
	double getOriginalF0_Offset() const;
	double getCorrelationCoefficient() const;
	double getRootMeanSquareError() const;
	double getSquareCorrelationCoefficient( const TamModelF0& tamF0 ) const;
	double getMeanSquareError( const TamModelF0& tamF0 ) const;
	std::tuple<double, double> getOptStats( const BoundaryVector& boundaries, const TargetVector& targets ) const;

	// operator called by optimizer
	double operator() (const DlibVector& arg) const;
	//BoundaryVector m_bounds;


private:
	// private member functions
	double costFunction(const TamModelF0& tamF0) const;
	static SampleTimes extractTimes(const TimeSignal& f0);
	static DlibVector signal2dlibVec(const TimeSignal& f0);

	// data members
	ParameterSet m_parameters;
	TimeSignal m_originalF0;
	BoundaryVector m_bounds;
	//BoundaryVector m_bounds;

	// store result
	TamModelF0 m_modelOptimalF0;
};