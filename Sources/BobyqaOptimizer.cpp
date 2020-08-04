#include <omp.h>
#include <dlib/optimization.h>
#include <dlib/threads.h>
#include "BobyqaOptimizer.h"

void BobyqaOptimizer::optimize(OptimizationProblem& op) const
{
	unsigned numTar = op.getPitchTargets().size();
	ParameterSet ps = op.getParameters();
	BoundaryVector initialBoundaries  = op.getBoundaries();

	// precalculate bounds
	double mmin = ps.meanSlope - ps.deltaSlope;
	double bmin = ps.meanOffset - ps.deltaOffset;
	double tmin = ps.meanTau - ps.deltaTau;
	double boundary_min = -1*ps.deltaBoundary; // adding the boundary optim
	double mmax = ps.meanSlope + ps.deltaSlope;
	double bmax = ps.meanOffset + ps.deltaOffset;
	double tmax = ps.meanTau + ps.deltaTau;
	double boundary_max = ps.deltaBoundary; // adding the boundary optim

	DlibVector lowerBound, upperBound;
	lowerBound.set_size(numTar * 4);
	upperBound.set_size(numTar * 4);

	for (unsigned i = 0; i < numTar; ++i)
	{
		lowerBound(4 * i + 0) = mmin;
		lowerBound(4 * i + 1) = bmin;
		lowerBound(4 * i + 2) = tmin;
		lowerBound(4 * i + 3) = boundary_min;
		upperBound(4 * i + 0) = mmax;
		upperBound(4 * i + 1) = bmax;
		upperBound(4 * i + 2) = tmax;
		upperBound(4 * i + 3) = boundary_max;
	}

	// optmization setup
	long npt(2 * lowerBound.size() + 1);	// number of interpolation points
	const double rho_begin = (std::min(std::min(std::min(mmax - mmin, bmax - bmin), tmax - tmin), boundary_max-boundary_min) - 1.0) / 2.0; // initial trust region radius
	const double rho_end(1e-6); // stopping trust region radius -> accuracy
	const long max_f_evals(1e5); // max number of objective function evaluations

	// initialize
	double fmin(1e6);
	DlibVector xtmp; double ftmp;
	dlib::mutex mu;

	// set up OpenMP
	int numThreads = omp_get_max_threads();
	omp_set_num_threads(numThreads);

#pragma omp parallel for schedule(dynamic)
	for (unsigned it = 0; it < RANDOMITERATIONS; ++it)
	{
		// random initialization
		DlibVector x;
		x.set_size(numTar * 4);
		for (unsigned i = 0; i < numTar; ++i)
		{
			x(4 * i + 0) = getRandomValue(mmin, mmax);
			x(4 * i + 1) = getRandomValue(bmin, bmax);
			x(4 * i + 2) = getRandomValue(tmin, tmax);
			x(4 * i + 3) = getRandomValue(boundary_min, boundary_max);
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
	TargetVector opt_targets;
	BoundaryVector opt_boundaries;
	for (unsigned i = 0; i < numTar; ++i)
	{
		opt_boundaries.push_back( initialBoundaries[i] + xtmp(4 * i +3)/1000 );

		PitchTarget pt;
		pt.slope = xtmp(4 * i + 0);
		pt.offset = xtmp(4 * i + 1);
		pt.tau = xtmp(4 * i + 2);
		pt.duration = ( initialBoundaries[i + 1] + xtmp(4 * (i+1) +3)/1000 ) - ( initialBoundaries[i] + xtmp(4 * i +3)/1000 );//op.getPitchTargets()[i].duration;
		opt_targets.push_back(pt);


	}
	opt_boundaries.push_back( initialBoundaries.at( numTar )  );

	// store optimum
	op.setOptimum( opt_boundaries, opt_targets );

	// DEBUG message
#ifdef DEBUG_MSG
	std::cout << "\t[optimize] mse = " << fmin << std::endl;
#endif
}

double BobyqaOptimizer::getRandomValue(const double min, const double max)
{
	return min + ((double)rand() / RAND_MAX) * (max - min);
}
