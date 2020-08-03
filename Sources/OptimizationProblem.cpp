#include "OptimizationProblem.h"


OptimizationProblem::OptimizationProblem(const ParameterSet& parameters, const TimeSignal& originalF0, const BoundaryVector& bounds)
	: m_parameters(parameters), m_originalF0(originalF0), m_bounds(bounds), m_modelOptimalF0(bounds, originalF0[0].value) {};

void OptimizationProblem::setOptimum(const TargetVector& targets)
{
	m_modelOptimalF0.setPitchTargets(targets);
}

ParameterSet OptimizationProblem::getParameters() const
{
	return m_parameters;
}

TimeSignal OptimizationProblem::getModelF0() const
{
	double samplingfrequency = SAMPLERATE; // Hz
	double dt = 1.0 / samplingfrequency;
	return m_modelOptimalF0.calculateF0(dt);
}

TargetVector OptimizationProblem::getPitchTargets() const
{
	return m_modelOptimalF0.getPitchTargets();
}

Sample OptimizationProblem::getOnset() const
{
	return m_modelOptimalF0.getOnset();
}

SampleTimes OptimizationProblem::extractTimes(const TimeSignal& f0)
{
	SampleTimes times;
	for (unsigned i = 0; i < f0.size(); ++i)
	{
		times.push_back(f0[i].time);
	}

	return times;
}

DlibVector OptimizationProblem::signal2dlibVec(const TimeSignal& f0)
{
	DlibVector values;
	values.set_size(f0.size());
	for (unsigned i = 0; i < f0.size(); ++i)
	{
		values(i) = f0[i].value;
	}

	return values;
}

double OptimizationProblem::getCorrelationCoefficient() const
{
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = m_modelOptimalF0.calculateF0(times);

	// Dlib format
	DlibVector orig = signal2dlibVec(m_originalF0);
	DlibVector model = signal2dlibVec(modelF0);

	// return correlation between filtered and original f0
	DlibVector x = orig - dlib::mean(orig);
	DlibVector y = model - dlib::mean(model);
	return (dlib::dot(x, y)) / ((std::sqrt(dlib::sum(dlib::squared(x)))) * (std::sqrt(dlib::sum(dlib::squared(y)))));

}

double OptimizationProblem::getRootMeanSquareError() const
{
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = m_modelOptimalF0.calculateF0(times);

	// Dlib format
	DlibVector orig = signal2dlibVec(m_originalF0);
	DlibVector model = signal2dlibVec(modelF0);

	// return RMSE between filtered and original f0
	return std::sqrt(dlib::mean(dlib::squared(model - orig)));
}

double OptimizationProblem::operator() (const DlibVector& arg) const
{
	// convert data
	TargetVector targets;
	BoundaryVector boundaries;
	for (unsigned i = 0; i < arg.size() / 4; ++i)
	{
		boundaries.push_back(m_bounds[i]);// + arg(4 * i +3));
		PitchTarget pt;
		pt.slope = arg(4 * i + 0);
		pt.offset = arg(4 * i + 1);
		pt.tau = arg(4 * i + 2);
		pt.duration = m_bounds[i + 1] - m_bounds[i];
		targets.push_back(pt);
	}

	// create model f0
	//TamModelF0 tamF0(m_bounds, m_originalF0[0].value);
	TamModelF0 tamF0(boundaries, m_originalF0[0].value);
	tamF0.setPitchTargets(targets);

	return costFunction(tamF0);
}

double OptimizationProblem::costFunction(const TamModelF0& tamF0) const
{
	// get model f0
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = tamF0.calculateF0(times);

	// calculate error
	double error = 0.0;
	for (unsigned i = 0; i < modelF0.size(); ++i)
	{
		error += std::pow((m_originalF0[i].value - modelF0[i].value), 2.0);
	}

	// calculate penalty term
	double penalty = 0.0;
	TargetVector targets = tamF0.getPitchTargets();
	for (unsigned i = 0; i < targets.size(); ++i)

	{
		penalty += (m_parameters.weightSlope * std::pow(targets[i].slope - m_parameters.meanSlope, 2.0));
		penalty += (m_parameters.weightOffset * std::pow(targets[i].offset - m_parameters.meanOffset, 2.0));
		penalty += (m_parameters.weightTau * std::pow(targets[i].tau - m_parameters.meanTau, 2.0));
	}

	return error + m_parameters.lambda * penalty;
}