#ifndef MODEL_H_
#define MODEL_H_

#include <cstdlib>
#include <time.h>
#include <vector>
#include <dlib/matrix.h>
#include <dlib/error.h>

// global constants
const unsigned FILTERORDER (5);			// TAM
const unsigned RANDOMITERATIONS (5);	// BOBYQA
const unsigned SAMPLERATE (200); 		// Hz

// sample of a discrete time signal
struct Sample
{
	double time;
	double value;
};

// discrete time signal types
typedef std::vector<double> SampleTimes;
typedef std::vector<Sample> TimeSignal;

// pitch target according to the TAM
struct PitchTarget
{
	double slope;
	double offset;
	double tau; // time constant
	double duration;
};

// vector of pitch targets
typedef std::vector<PitchTarget> TargetVector;

// vector of syllable bounds
typedef std::vector<double> BoundVector;

class TamModelF0 {
public:
	// constructors
	TamModelF0 (const BoundVector &bounds, const double onsetValue);

	// public member functions
	void setPitchTargets(const TargetVector &targets);
	TimeSignal calculateF0(const double samplingPeriod) const;
	TimeSignal calculateF0(const SampleTimes &times) const;

	TargetVector getPitchTargets() const;
	Sample getOnset() const;

private:
	// private member functions
	TimeSignal applyFilter(const SampleTimes &times) const;

	// data members
	Sample m_onset;
	TargetVector m_targets;
};

// vector types for CdlpFilter
typedef std::vector<double> FilterState;
typedef std::vector<double> FilterCoefficients;

// Nth order critical damped low pass filter for target approximation
class CdlpFilter {
public:
	// constructors
	CdlpFilter (const unsigned order=FILTERORDER) : m_filterOrder(order) {};

	// public member functions
	TimeSignal response (const SampleTimes &sampleTimes, const TargetVector &targets, const Sample onset) const;

private:
	// private member functions
	FilterCoefficients calculateCoefficients (const PitchTarget &target, const FilterState &state) const;
	FilterState calculateState (const FilterState &state, const double time, const double startTime, const PitchTarget &target) const;
	static double binomial (const unsigned n, const unsigned k);
	static double factorial (unsigned n);

	// data members
	unsigned m_filterOrder;
};

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
typedef dlib::matrix<double,0,1> DlibVector;

// optimization problem for calculating pitch targets
class OptimizationProblem {
public:
	// constructors
	OptimizationProblem (const ParameterSet &parameters, const TimeSignal &originalF0, const BoundVector &bounds);

	// public member functions
	void setOptimum(const TargetVector &targets);

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
	double costFunction(const TamModelF0 &tamF0) const;
	static SampleTimes extractTimes(const TimeSignal &f0);
	static DlibVector signal2dlibVec(const TimeSignal &f0);

	// data members
	ParameterSet m_parameters;
	TimeSignal m_originalF0;
	BoundVector m_bounds;

	// store result
	TamModelF0 m_modelOptimalF0;
};

// solver for an optimization problem utilizing BOBYQA algorithm
class BobyqaOptimizer {
public:
	// constructors
	BobyqaOptimizer() { srand (time(NULL)); };

	// public member functions
	void optimize(OptimizationProblem& op) const;

private:
	// private member functions
	static double getRandomValue (const double min, const double max);
};

#endif /* MODEL_H_ */
