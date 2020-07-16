#include <omp.h>
#include <math.h>
#include <string>
#include <sstream>
#include <dlib/threads.h>
#include <dlib/optimization.h>
#include "model.h"

TamModelF0::TamModelF0 (const BoundVector &bounds, const double onsetValue)
{
	// determine onset
	m_onset.time = bounds[0];
	m_onset.value = onsetValue;

	// determine target durations
	for (std::vector<double>::const_iterator bd = bounds.begin()+1; bd != bounds.end(); ++bd)
	{
		double duration = *bd - *(bd-1);
		PitchTarget pt = {0.0, 0.0, 0.0, duration};
		m_targets.push_back(pt);
	}
}

void TamModelF0::setPitchTargets(const TargetVector &targets)
{
	m_targets = targets;
}

TimeSignal TamModelF0::calculateF0(const double samplingPeriod) const
{
	// get length of signal
	double start = m_onset.time;
	double end = start;
	for (std::vector<PitchTarget>::const_iterator pt = m_targets.begin(); pt != m_targets.end(); ++pt)
	{
		end += pt->duration;
	}

	// sampling
	SampleTimes times;
	for (double t=start; t<=end; t+=samplingPeriod)
	{
		times.push_back(t);
	}

	// return sampled f0
	return applyFilter(times);
}

TimeSignal TamModelF0::calculateF0(const SampleTimes &times) const
{
	// return sampled f0
	return applyFilter(times);
}

TargetVector TamModelF0::getPitchTargets() const
{
	return m_targets;
}

Sample TamModelF0::getOnset() const
{
	return m_onset;
}

TimeSignal TamModelF0::applyFilter(const SampleTimes &times) const
{
	CdlpFilter lowPass(FILTERORDER);
	return lowPass.response(times,m_targets,m_onset);
}

TimeSignal CdlpFilter::response (const SampleTimes &sampleTimes, const TargetVector &targets, const Sample onset) const
{
	// local container
	TimeSignal f0;

	// keep state at syllable bound
	FilterState currentState;
	currentState.push_back(onset.value);
	for (unsigned i=1; i<m_filterOrder; ++i)
	{
		currentState.push_back(0.0);
	}

	// keep index of current sample
	unsigned sampleIndex (0);

	// track syllable bounds of each target
	double bBegin = onset.time;
	double bEnd = bBegin;

	for (std::vector<PitchTarget>::const_iterator pt = targets.begin(); pt != targets.end(); ++pt)
	{
		// update bounds
		bBegin = bEnd;
		bEnd = bBegin + pt->duration;

		// filter coefficients
		FilterCoefficients c = calculateCoefficients(*pt, currentState);

		while (sampleTimes[sampleIndex] <= bEnd)
		{
			double acc (0.0);
			double t = sampleTimes[sampleIndex] - bBegin;	// current samplePoint, time shift
			for (unsigned n=0; n<m_filterOrder; ++n)
			{
				acc += (c[n] * std::pow(t,n));
			}

			double time = sampleTimes[sampleIndex];
			double value = acc * std::exp(-(1000.0/pt->tau)*t) + pt->slope*t + pt->offset;

			Sample s = {time,value};
			f0.push_back(s);
			sampleIndex++;

			if (sampleIndex >= sampleTimes.size())
			{
				break;
				std::ostringstream msg;
				msg << "[calc_f0] Sample index exceeds sample time vector! " << sampleTimes.size();
				throw dlib::error(msg.str());
			}
		}

		// update filter state
		currentState = calculateState(currentState, bEnd, bBegin, *pt);
	}

	return f0;
}

FilterCoefficients CdlpFilter::calculateCoefficients (const PitchTarget &target, const FilterState &state) const
{
	FilterCoefficients coeffs (state.size(), 0.0);

	if (coeffs.size() != m_filterOrder)
	{
		std::ostringstream msg;
		msg << "[calculateCoefficients] Wrong size of coefficient vector! " << coeffs.size() << " != " << m_filterOrder;
		throw dlib::error(msg.str());
	}

	coeffs[0] = state[0] - target.offset;	// 0th coefficient
	for (unsigned n=1; n<m_filterOrder; ++n)	// other coefficients
	{
		double acc (0.0);
		for (unsigned i=0; i<n; ++i)
		{
			acc += (coeffs[i]*std::pow(-(1000.0/target.tau),n-i)*binomial(n,i)*factorial(i));
		}

		if (n==1)
		{
			acc += target.slope; // adaption for linear targets; minus changes in following term!
		}

		coeffs[n] = (state[n] - acc)/factorial(n);
	}

	return coeffs;
}

FilterState CdlpFilter::calculateState (const FilterState &state, const double time, const double startTime, const PitchTarget &target) const
{
	// setup
	double t (time - startTime); // sample time
	const unsigned& N (m_filterOrder);
	FilterState stateUpdate(N);

	// filter coefficients
	FilterCoefficients c = calculateCoefficients(target, state);

	for (unsigned n=0; n<N; ++n)
	{
		// calculate value of nth derivative
		double acc (0.0);
		for (unsigned i=0; i<N; ++i)
		{
			// pre-calculate q-value
			double q (0.0);
			for (unsigned k=0; k<std::min(N-i,n+1); ++k)
			{
				q += (std::pow(-(1000.0/target.tau),n-k)*binomial(n,k)*c[i+k]*factorial(k+i)/factorial(i));
			}

			acc += (std::pow(t,i)*q);
		}

		stateUpdate[n] = acc * std::exp(-(1000.0/target.tau)*t);
	}

	// correction for linear targets
	if (N > 1)
	{
		stateUpdate[0] += (target.offset+target.slope*t);
	}
	if (N > 2)
	{
		stateUpdate[1] += target.slope;
	}

	return stateUpdate;
}

double CdlpFilter::binomial (const unsigned n, const unsigned k)
{
	double result = 1;
	unsigned int tmp = k;

	if ( tmp > n - tmp )
		tmp = n - tmp;

	for (unsigned i = 0; i < tmp; ++i)
	{
		result *= (n - i);
		result /= (i + 1);
	}

	return result;
}

double CdlpFilter::factorial (unsigned n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

OptimizationProblem::OptimizationProblem (const ParameterSet &parameters, const TimeSignal &originalF0, const BoundVector &bounds)
	: m_parameters(parameters),  m_originalF0(originalF0), m_bounds(bounds), m_modelOptimalF0(bounds, originalF0[0].value) {};

void OptimizationProblem::setOptimum(const TargetVector &targets)
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
	double dt = 1.0/samplingfrequency;
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

SampleTimes OptimizationProblem::extractTimes(const TimeSignal &f0)
{
	SampleTimes times;
	for (unsigned i=0; i<f0.size(); ++i)
	{
		times.push_back(f0[i].time);
	}

	return times;
}

DlibVector OptimizationProblem::signal2dlibVec(const TimeSignal &f0)
{
	DlibVector values;
	values.set_size(f0.size());
	for (unsigned i=0; i<f0.size(); ++i)
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
	return (dlib::dot(x,y)) / ( (std::sqrt( dlib::sum(dlib::squared(x))) ) * (std::sqrt( dlib::sum(dlib::squared(y))) ) );

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
	for (unsigned i=0; i<arg.size()/3; ++i)
	{
		PitchTarget pt;
		pt.slope = arg(3*i+0);
		pt.offset = arg(3*i+1);
		pt.tau = arg(3*i+2);
		pt.duration = m_bounds[i+1] - m_bounds[i];
		targets.push_back(pt);
	}

	// create model f0
	TamModelF0 tamF0 (m_bounds, m_originalF0[0].value);
	tamF0.setPitchTargets(targets);

	return costFunction(tamF0);
}

double OptimizationProblem::costFunction(const TamModelF0 &tamF0) const
{
	// get model f0
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = tamF0.calculateF0(times);

	// calculate error
	double error = 0.0;
	for (unsigned i=0; i<modelF0.size(); ++i)
	{
		error += std::pow((m_originalF0[i].value - modelF0[i].value),2.0);
	}

	// calculate penalty term
	double penalty = 0.0;
	TargetVector targets = tamF0.getPitchTargets();
	for (unsigned i=0; i<targets.size(); ++i)

	{
		penalty += (m_parameters.weightSlope * std::pow(targets[i].slope - m_parameters.meanSlope, 2.0));
		penalty += (m_parameters.weightOffset * std::pow(targets[i].offset - m_parameters.meanOffset, 2.0));
		penalty += (m_parameters.weightTau * std::pow(targets[i].tau - m_parameters.meanTau, 2.0));
	}

	return error + m_parameters.lambda*penalty;
}

void BobyqaOptimizer::optimize(OptimizationProblem& op) const
{
	unsigned numTar = op.getPitchTargets().size();
	ParameterSet ps = op.getParameters();

	// precalculate bounds
	double mmin = ps.meanSlope-ps.deltaSlope;
	double bmin = ps.meanOffset-ps.deltaOffset;
	double tmin = ps.meanTau-ps.deltaTau;
	double mmax = ps.meanSlope+ps.deltaSlope;
	double bmax = ps.meanOffset+ps.deltaOffset;
	double tmax = ps.meanTau+ps.deltaTau;

	DlibVector lowerBound, upperBound;
	lowerBound.set_size(numTar*3);
	upperBound.set_size(numTar*3);

	for (unsigned i=0; i<numTar; ++i)
	{
		lowerBound(3*i+0) = mmin;
		lowerBound(3*i+1) = bmin;
		lowerBound(3*i+2) = tmin;
		upperBound(3*i+0) = mmax;
		upperBound(3*i+1) = bmax;
		upperBound(3*i+2) = tmax;
	}

	// optmization setup
	long npt (2*lowerBound.size()+1);	// number of interpolation points
	const double rho_begin = (std::min(std::min(mmax-mmin, bmax-bmin),tmax-tmin)-1.0)/2.0; // initial trust region radius
	const double rho_end (1e-6); // stopping trust region radius -> accuracy
	const long max_f_evals (1e5); // max number of objective function evaluations

	// initialize
	double fmin (1e6);
	DlibVector xtmp; double ftmp;
	dlib::mutex mu;

	// set up OpenMP
	int numThreads = omp_get_max_threads();
	omp_set_num_threads(numThreads);

	#pragma omp parallel for schedule(dynamic)
	for (unsigned it=0; it<RANDOMITERATIONS; ++it)
	{
		// random initialization
		DlibVector x;
		x.set_size(numTar*3);
		for (unsigned i=0; i<numTar; ++i)
		{
			x(3*i+0) = getRandomValue(mmin, mmax);
			x(3*i+1) = getRandomValue(bmin, bmax);
			x(3*i+2) = getRandomValue(tmin, tmax);
		}

		try
		{
			// optimization algorithm: BOBYQA
			ftmp = dlib::find_min_bobyqa(op,x,npt,lowerBound,upperBound,rho_begin,rho_end,max_f_evals);
		}
		catch (dlib::bobyqa_failure& err)
		{
			// DEBUG message
			#ifdef DEBUG_MSG
			std::cout << "\t[optimize] WARNING: no convergence during optimization in iteration: " << it << std::endl << err.info << std::endl;
			#endif
		}

		// write optimization results back
		#pragma omp critical (updateMinValue)
		if (ftmp < fmin && ftmp > 0.0)	// opt returns 0 by error
		{
			fmin = ftmp;
			xtmp = x;
		}
	}

	if (fmin == 1e6)
	{
		throw dlib::error("[optimize] BOBYQA algorithms didn't converge! Try to increase number of evaluations");
	}

	// convert result to TargetVector
	TargetVector opt;
	for (unsigned i=0; i<numTar; ++i)
	{
		PitchTarget pt;
		pt.slope = xtmp(3*i+0);
		pt.offset = xtmp(3*i+1);
		pt.tau = xtmp(3*i+2);
		pt.duration = op.getPitchTargets()[i].duration;
		opt.push_back(pt);
	}

	// store optimum
	op.setOptimum(opt);

	// DEBUG message
	#ifdef DEBUG_MSG
	std::cout << "\t[optimize] mse = " << fmin << std::endl;
	#endif
}

double BobyqaOptimizer::getRandomValue (const double min, const double max)
{
	return min + ((double)rand()/RAND_MAX)*(max-min);
}
