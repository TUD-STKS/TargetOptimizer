#include <omp.h>
#include <dlib/optimization.h>
#include <dlib/threads.h>
#include "BobyqaOptimizer.h"
#include <iostream> //eclude again


void BobyqaOptimizer::optimize( OptimizationProblem& op, OptimizerOptions optOpt ) const
{
	//std::cout << "omp cancel " << omp_get_cancellation()  << std::endl;
	unsigned number_Targets = op.getPitchTargets().size();
	ParameterSet ps  = op.getParameters();
	bool optimizeBoundaries = ps.optimizeBoundaries;//(ps.deltaBoundary != 0);
	BoundaryVector initialBoundaries  = op.getBoundaries();
	BoundaryVector tmpBoundaries = initialBoundaries;
	BoundaryVector optBoundaries;
	TargetVector   tmpTargets = op.getPitchTargets();


	int number_optVar = ps.numberOptVar; // Optimize the 3 target parameters by default

	double tmpMSE, tmpSCC;

	const unsigned RANDOMITERATIONS = optOpt.maxIterations;
	const double MSE_Threshold = optOpt.rmseThreshold * optOpt.rmseThreshold;
	const double SCC_Threshold = optOpt.correlationThreshold * optOpt.correlationThreshold;
	bool useMSEThreshold = optOpt.useRmseThreshold;
	bool useSCCThreshold = optOpt.useCorrelationThreshold;
	bool use_MSE_SCC_Threshold = useMSEThreshold && useSCCThreshold;

	if (useMSEThreshold)
	{
		useMSEThreshold = false;
		useSCCThreshold = false;
	}

	//srand(1);


	// precalculate search space bounds (ssp_bounds)


	std::vector<double> min_bounds;
	std::vector<double> max_bounds;
	min_bounds.push_back( ps.meanSlope - ps.deltaSlope ); //mmin
	min_bounds.push_back( ps.meanOffset - ps.deltaOffset ); //bmin
	min_bounds.push_back( ps.meanTau - ps.deltaTau ); //tmin
	max_bounds.push_back( ps.meanSlope + ps.deltaSlope ); // mmax
	max_bounds.push_back( ps.meanOffset + ps.deltaOffset ); //bmax
	max_bounds.push_back( ps.meanTau + ps.deltaTau ); //tmax

	if (optimizeBoundaries)
	{
		min_bounds.push_back( -1*ps.deltaBoundary ); //boundary_min
		max_bounds.push_back(    ps.deltaBoundary ); //boundary_max
	}

	DlibVector lowerBound, upperBound;
	lowerBound.set_size(number_Targets * number_optVar);
	upperBound.set_size(number_Targets * number_optVar);

	for (unsigned i = 0; i < number_Targets; ++i)
	{
		for (unsigned ssp_bound = 0; ssp_bound < number_optVar; ++ssp_bound)
		{
			lowerBound(number_optVar * i + ssp_bound) = min_bounds.at(ssp_bound);
			upperBound(number_optVar * i + ssp_bound) = max_bounds.at(ssp_bound);
		}
		//lowerBound(number_optVar * i + 0) = mmin;
		//lowerBound(number_optVar * i + 1) = bmin;
		//lowerBound(number_optVar * i + 2) = tmin;
		//upperBound(number_optVar * i + 0) = mmax;
		//upperBound(number_optVar * i + 1) = bmax;
		//upperBound(number_optVar * i + 2) = tmax;

	}

//	auto min_slope_offset_tau = std::min(std::min(mmax - mmin, bmax - bmin), tmax - tmin)
//	double trustRegion = std::for_each(min_bounds.begin(), min_bounds.end(), [&r = x, x = x+1]()-> double {
//            a = std::min( a, b )
//            return std::min(a, b);


	double trustRegion = max_bounds.at(0)-min_bounds.at(0);
	for (unsigned i =1; i < number_optVar; ++i)
	{
		trustRegion = std::min(trustRegion, max_bounds.at(i)-min_bounds.at(i));
	}

	// optmization setup
	long npt(2 * lowerBound.size() + 1);	// number of interpolation points
	const double rho_begin = (trustRegion - 1.0) / 2.0; // initial trust region radius
	//std::cout << "rho_begint"<< rho_begint << std::endl;

	//const double rho_begin = (std::min(std::min(max_bounds.at(0)-min_bounds.at(0), max_bounds.at(1)-min_bounds.at(1)), max_bounds.at(2)-min_bounds.at(2)) -1.0) /2.0;
	std::cout << "rho_begin"<< rho_begin << std::endl;
	const double rho_end(1e-6); // stopping trust region radius -> accuracy
	const long max_f_evals(1e6); // max number of objective function evaluations

	// initialize
	double fmin(1e6);
	DlibVector xtmp; double ftmp;
	dlib::mutex mu;

	// set up OpenMP
	int numThreads = omp_get_max_threads();
	omp_set_num_threads(numThreads);
//std::cout << "BobyqaOptimizer line 84" << std::endl;
	bool SearchFinished = false;
	int boundaryResetCounter = 0;
	int iteration = 0;
//#pragma omp parallel for schedule(dynamic)

	for (unsigned it = 0; it < RANDOMITERATIONS; ++it)
	{
		if (!SearchFinished)
		{
			std::cout << '\r' << "Iteration nr: "<< iteration << std::flush;
			// random initialization
			DlibVector x;
			x.set_size(number_Targets * number_optVar);
			for (unsigned i = 0; i < number_Targets; ++i)
			{
				for (unsigned ssp_bound = 0; ssp_bound < number_optVar; ++ssp_bound)
				{
					x(number_optVar * i + ssp_bound) = getRandomValue( min_bounds.at(ssp_bound), max_bounds.at(ssp_bound) );
				}
				//x(4 * i + 0) = getRandomValue(mmin, mmax);
				//x(4 * i + 1) = getRandomValue(bmin, bmax);
				//x(4 * i + 2) = getRandomValue(tmin, tmax);
				//x(4 * i + 3) = getRandomValue(boundary_min, boundary_max);
			}
			try
			{
				// optimization algorithm: BOBYQA
				ftmp = dlib::find_min_bobyqa(op, x, npt, lowerBound, upperBound, rho_begin, rho_end, max_f_evals);
				//std::cout << "ftmp" << ftmp << std::endl;
			}
			catch (dlib::bobyqa_failure & err)
			{
				// DEBUG message
	#ifdef DEBUG_MSG
				std::cout << "\t[optimize] WARNING: no convergence during optimization in iteration: " << it << std::endl << err.info << std::endl;
	#endif
			}
			// write optimization results back
//#pragma omp critical (updateMinValue)



			if (ftmp < fmin && ftmp > 0.0)	// opt returns 0 by error
			{
				fmin = ftmp;
				xtmp = x;
				if (optimizeBoundaries)
				{
					for (unsigned i = 0; i <= number_Targets; ++i)
					{
						tmpBoundaries.at(i) += xtmp(number_optVar * i + 3)/1000; //divide by 1000 because delta is ms
						if ( (i==0) && (tmpBoundaries.at(0) > op.getOriginalF0_Onset()) )
						{
							tmpBoundaries.at(0) = op.getOriginalF0_Onset();
						}
						if ( (i==number_Targets) && (tmpBoundaries.back() < op.getOriginalF0_Offset()) )
						{
							tmpBoundaries.back() = op.getOriginalF0_Offset();
						}
					}
					op.setBoundaries( tmpBoundaries );
					optBoundaries = tmpBoundaries;
				}//else{
					//tmpBoundaries = initialBoundaries; Ist sowieso so, da bei optBound= FAlse die tmpbounds nicht geändert werden
				//}
				for (unsigned i = 0; i < number_Targets; ++i)
				{
					PitchTarget pt;
					pt.slope = xtmp(number_optVar * i + 0);
					pt.offset = xtmp(number_optVar * i + 1);
					pt.tau = xtmp(number_optVar * i + 2);
					pt.duration = tmpBoundaries.at(i+1) - tmpBoundaries.at(i);
					tmpTargets.at(i) = pt;
				}
				std::tie(tmpMSE, tmpSCC) = op.getOptStats( tmpBoundaries, tmpTargets );
				if ( (useMSEThreshold && (tmpMSE < MSE_Threshold)) || (useSCCThreshold && (tmpSCC > SCC_Threshold)) )
				{
					SearchFinished = true;
					std::cout << "  fin reached " << std::endl;
				}else if( use_MSE_SCC_Threshold && (tmpMSE < MSE_Threshold) && (tmpSCC > SCC_Threshold) )
				{
					SearchFinished = true;
					std::cout << "  fin2 reached " << std::endl;
				}
			}

			if (boundaryResetCounter == 10)
			{
				tmpBoundaries = initialBoundaries;
				op.setBoundaries( initialBoundaries );
				boundaryResetCounter = 0;
				std::cout << "  reset boundaries " << std::endl;
			}

			++boundaryResetCounter;
			++iteration;
		}
	}

	if (fmin == 1e6)
	{
		throw dlib::error("[optimize] BOBYQA algorithms didn't converge! Try to increase number of evaluations");
	}

	// convert result to TargetVector
	TargetVector optTargets;

	if (!optimizeBoundaries)
	{
		optBoundaries = initialBoundaries;
	}
std::cout << "" << std::endl;
std::cout << "BobyqaOptimizer line 148, xtmp.size: "<< xtmp.size() << std::endl;
	//BoundaryVector opt_boundaries = tmpBoundaries;
	for (unsigned i =0; i < xtmp.size(); ++i)
	{
		std::cout << "xtmp at: " << i << " is: " << xtmp(i) << std::endl;
	}
	for (unsigned i = 0; i < number_Targets; ++i)
	{
		//opt_boundaries.push_back( initialBoundaries[i] + xtmp(4 * i +3)/1000 );

		PitchTarget pt;
		pt.slope = xtmp(number_optVar * i + 0);
		pt.offset = xtmp(number_optVar * i + 1);
		pt.tau = xtmp(number_optVar * i + 2);
		//pt.duration = ( initialBoundaries[i + 1] + xtmp(4 * (i+1) +3)/1000 ) - ( initialBoundaries[i] + xtmp(4 * i +3)/1000 );//op.getPitchTargets()[i].duration;
		pt.duration = optBoundaries.at(i + 1)  - optBoundaries.at(i);
		optTargets.push_back(pt);


	}
	//opt_boundaries.push_back( initialBoundaries.at( number_Targets )  );

	// store optimum
	op.setOptimum( optBoundaries, optTargets );
//std::cout << "BobyqaOptimizer line 168" << std::endl;
	// DEBUG message
#ifdef DEBUG_MSG
	std::cout << "\t[optimize] mse = " << fmin << std::endl;
#endif
}



double BobyqaOptimizer::getRandomValue(const double min, const double max)
{
	return min + ((double)rand() / RAND_MAX) * (max - min);
}
