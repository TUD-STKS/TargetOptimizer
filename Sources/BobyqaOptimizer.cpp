#include <omp.h>
#include <dlib/optimization.h>
#include <dlib/threads.h>
#include "BobyqaOptimizer.h"

void BobyqaOptimizer::optimize(OptimizationProblem& op) const
{
	unsigned numTar = op.getPitchTargets().size();
	ParameterSet ps = op.getParameters();

	// precalculate bounds
	double mmin = ps.meanSlope - ps.deltaSlope;
	double bmin = ps.meanOffset - ps.deltaOffset;
	double tmin = ps.meanTau - ps.deltaTau;
	double mmax = ps.meanSlope + ps.deltaSlope;
	double bmax = ps.meanOffset + ps.deltaOffset;
	double tmax = ps.meanTau + ps.deltaTau;

	DlibVector lowerBound, upperBound;
	lowerBound.set_size(numTar * 3);
	upperBound.set_size(numTar * 3);

	for (unsigned i = 0; i < numTar; ++i)
	{
		lowerBound(3 * i + 0) = mmin;
		lowerBound(3 * i + 1) = bmin;
		lowerBound(3 * i + 2) = tmin;
		upperBound(3 * i + 0) = mmax;
		upperBound(3 * i + 1) = bmax;
		upperBound(3 * i + 2) = tmax;
	}

	// optmization setup
	long npt(2 * lowerBound.size() + 1);	// number of interpolation points
	const double rho_begin = (std::min(std::min(mmax - mmin, bmax - bmin), tmax - tmin) - 1.0) / 2.0; // initial trust region radius
	const double rho_end(1e-6); // stopping trust region radius -> accuracy
	const long max_f_evals(1e5); // max number of objective function evaluations

	// initialize
	double fmin(1e6);
	DlibVector xtmp; double ftmp;
	dlib::mutex mu;

	// set up OpenMP
	int numThreads = omp_get_max_threads();
	omp_set_num_threads(numThreads);

	int numIterationsDone(0);
#pragma omp parallel for schedule(dynamic)
	for (unsigned it = 0; it < RANDOMITERATIONS; ++it)
	{
		// random initialization
		DlibVector x;
		x.set_size(numTar * 3);
		for (unsigned i = 0; i < numTar; ++i)
		{
			x(3 * i + 0) = getRandomValue(mmin, mmax);
			x(3 * i + 1) = getRandomValue(bmin, bmax);
			x(3 * i + 2) = getRandomValue(tmin, tmax);
		}

		try
		{
			// optimization algorithm: BOBYQA
			ftmp = dlib::find_min_bobyqa(op, x, npt, lowerBound, upperBound, rho_begin, rho_end, max_f_evals);
		}
		catch (dlib::bobyqa_failure & err)
		{
			// DEBUG message
#ifdef DEBUG_MSG
			std::cout << "\t[optimize] WARNING: no convergence during optimization in iteration: " << it << std::endl << err.info << std::endl;
#endif
		}

		// write optimization results back
#pragma omp critical (updateMinValue)
		{
			if (ftmp < fmin && ftmp > 0.0)	// opt returns 0 by error
			{
				fmin = ftmp;
				xtmp = x;

			}
			std::cout << "Iteration " << ++numIterationsDone << " of " << RANDOMITERATIONS << " finished." << std::endl;
		}
	}

	if (fmin == 1e6)
	{
		throw dlib::error("[optimize] BOBYQA algorithms didn't converge! Try to increase number of evaluations");
	}

	// convert result to TargetVector
	TargetVector opt;
	for (unsigned i = 0; i < numTar; ++i)
	{
		PitchTarget pt;
		pt.slope = xtmp(3 * i + 0);
		pt.offset = xtmp(3 * i + 1);
		pt.tau = xtmp(3 * i + 2);
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

double BobyqaOptimizer::getRandomValue(const double min, const double max)
{
	return min + ((double)rand() / RAND_MAX) * (max - min);
}
