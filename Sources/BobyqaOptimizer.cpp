#include <omp.h>
#include <dlib/optimization.h>
#include <dlib/threads.h>
#include "BobyqaOptimizer.h"
#include <iostream> //eclude again
//#include <string>
#include <fstream>
#include <chrono>
#include <ctime>


void BobyqaOptimizer::optimize(OptimizationProblem& op, OptimizerOptions optOpt) const
{
	//std::cout << "omp cancel " << omp_get_cancellation()  << std::endl;
	unsigned number_Targets = op.getPitchTargets().size();
	ParameterSet ps = op.getParameters();
	bool optimizeBoundaries = ps.searchSpaceParameters.optimizeBoundaries;//(ps.deltaBoundary != 0);
	BoundaryVector initialBoundaries = op.getBoundaries();
	initialBoundaries.back() = op.getOriginalF0_Offset();
	BoundaryVector tmpBoundaries = initialBoundaries;
	BoundaryVector optBoundaries;
	TargetVector   tmpTargets = op.getPitchTargets();


	int number_optVar = ps.searchSpaceParameters.numberOptVar; // Optimize the 3 target parameters by default

	double tmpMSE = 1e6;
	//double minMSE = 1e6;
	double tmpSCC = 0;
	//double minSCC = 0;
	//double minMSE = 1e6; //this is the mse corresponding to

	const unsigned RANDOMITERATIONS = optOpt.maxIterations;
	const int patience = optOpt.patience;
	double epsilon = optOpt.epsilon;
	bool useEarlyStopping = optOpt.useEarlyStopping;

	const long max_f_evals = optOpt.maxCostEvaluations;
	const double rho_end = optOpt.rhoEnd;

	//bool writeLOG = (LOG_PATH != "");

	bool relativeDelta = true;


	//srand(1);


	// precalculate search space bounds (ssp_bounds)


	std::vector<double> min_bounds;
	std::vector<double> max_bounds;
	min_bounds.push_back(ps.searchSpaceParameters.meanSlope - ps.searchSpaceParameters.deltaSlope); //mmin
	min_bounds.push_back(ps.searchSpaceParameters.meanOffset - ps.searchSpaceParameters.deltaOffset); //bmin
	min_bounds.push_back(ps.searchSpaceParameters.meanTau - ps.searchSpaceParameters.deltaTau); //tmin
	max_bounds.push_back(ps.searchSpaceParameters.meanSlope + ps.searchSpaceParameters.deltaSlope); // mmax
	max_bounds.push_back(ps.searchSpaceParameters.meanOffset + ps.searchSpaceParameters.deltaOffset); //bmax
	max_bounds.push_back(ps.searchSpaceParameters.meanTau + ps.searchSpaceParameters.deltaTau); //tmax

	if (optimizeBoundaries)
	{
		min_bounds.push_back(-1 * ps.searchSpaceParameters.deltaBoundary); //boundary_min
		max_bounds.push_back(ps.searchSpaceParameters.deltaBoundary); //boundary_max
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


	double trustRegion = max_bounds.at(0) - min_bounds.at(0);
	for (unsigned i = 1; i < number_optVar; ++i)
	{
		trustRegion = std::min(trustRegion, max_bounds.at(i) - min_bounds.at(i));
	}

	// optmization setup
	long npt(2 * lowerBound.size() + 1);	// number of interpolation points
	const double rho_begin = (trustRegion - 1.0) / 2.0; // initial trust region radius
	//std::cout << "rho_begint"<< rho_begint << std::endl;

	//const double rho_begin = (std::min(std::min(max_bounds.at(0)-min_bounds.at(0), max_bounds.at(1)-min_bounds.at(1)), max_bounds.at(2)-min_bounds.at(2)) -1.0) /2.0;
	std::cout << "rho_begin" << rho_begin << std::endl;
	//const double rho_end(1e-6); // stopping trust region radius -> accuracy
	//const long max_f_evals(1e6); // max number of objective function evaluations

	// initialize
	double fmin(1e6);
	DlibVector xtmp;
	dlib::mutex mu;

	// set up OpenMP
	int numThreads = omp_get_max_threads();
	omp_set_num_threads(numThreads);
	//std::cout << "BobyqaOptimizer line 84" << std::endl;
	bool SearchFinished = false;
	int boundaryResetCounter = 0;
	int iteration = 0;
	int convergence = 0;

	std::vector<double> ftmp_vector(RANDOMITERATIONS, -1.0);
	//v.size(); //returns RANDOMITERATIONS

	std::ofstream LOG;

	//	if ( writeLOG )
	//	{
	//  		LOG.open ( LOG_PATH );
	//  		LOG << "fmin ftmp tmpMSE tmpSCC time" << "\n";
	//	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	#pragma omp parallel for schedule(dynamic)

	for (int it = 0; it < RANDOMITERATIONS; ++it)
	{
		double ftmp;
		if (!SearchFinished)
		{
			DlibVector x;
			#pragma omp critical (getRandomValues)
			{
				std::cout << "random iteration: " << it << " begins." << std::endl;
				// random initialization
				srand(it);
				x.set_size(number_Targets * number_optVar);
				for (unsigned i = 0; i < number_Targets; ++i)
				{
					for (unsigned ssp_bound = 0; ssp_bound < number_optVar; ++ssp_bound)
					{
						x(number_optVar * i + ssp_bound) = getRandomValue(min_bounds.at(ssp_bound), max_bounds.at(ssp_bound));
					}
				}
			}
			try
			{
				// optimization algorithm: BOBYQA
				//std::cout << "x: " << x << std::endl;
				ftmp = dlib::find_min_bobyqa(op, x, npt, lowerBound, upperBound, rho_begin, rho_end, max_f_evals);
			}
			catch (dlib::bobyqa_failure & err)
			{
				// DEBUG message
				//#ifdef DEBUG_MSG
				std::cout << "\t[optimize] WARNING: no convergence during optimization in iteration: " << it << std::endl << err.info << std::endl;
				//#endif
			}
			// write optimization results back
			#pragma omp critical (updateMinValue)
			{
				ftmp_vector.at(it) = ftmp;
				std::cout << "Iteration nr: " << it << " fmin: " << fmin << " ftmp: " << ftmp << std::endl;
				if (ftmp < fmin && ftmp > 0.0)	// opt returns 0 by error
				{
					if (fmin - ftmp < fmin * epsilon) { ++convergence; }
					else { convergence = 0; }
					fmin = ftmp;
					xtmp = x;
					//std::cout << "ftmp: " << ftmp << std::endl;
				}
				else
				{
					++convergence;
				}
				//if ( useEarlyStopping )
				//{
				//	if ( convergence >= patience )
				//	{
				//		SearchFinished = true;
				//		//std::cout << "" << std::endl;
				//		//std::cout << "Search stopped early!" << std::endl;
				//	}else{
				//		SearchFinished = false;
				//	}
				//}
				++iteration;
				auto end = std::chrono::system_clock::now();
				std::time_t end_time = std::chrono::system_clock::to_time_t(end);
				std::cout << "random iteration: " << it << " ended. System time: " << std::ctime(&end_time) << std::endl;
			}
		}
	}





	if (fmin == 1e6)
	{
		std::cout << "  dlib error!!!!!!!!!!!!! " << std::endl;
		throw dlib::error("[optimize] BOBYQA algorithms didn't converge! Try to increase number of evaluations");
	}

	// convert result to TargetVector
	TargetVector optTargets;

	if (!optimizeBoundaries)
	{
		optBoundaries = initialBoundaries;
	}
	else
	{

		//tmpBoundaries.back() = op.getOriginalF0_Offset();
		for (unsigned i = 0; i < number_Targets; ++i)
		{
			if (relativeDelta) {
				if (xtmp(number_optVar * i + 3) >= 0)
				{
					tmpBoundaries.at(i) += xtmp(number_optVar * i + 3) * (initialBoundaries.at(i + 1) - initialBoundaries.at(i)) * 0.01;
				}
				else
				{
					if (i == 0)
					{
						tmpBoundaries.at(i) += xtmp(number_optVar * i + 3) * 0.1 * 0.01;
					}
					else
					{
						tmpBoundaries.at(i) += xtmp(number_optVar * i + 3) * (initialBoundaries.at(i) - initialBoundaries.at(i - 1)) * 0.01;
					}
				}
			}
			else // for absolute delta (currently disabled)
			{
				tmpBoundaries.at(i) += xtmp(number_optVar * i + 3) / 1000; //divide by 1000 because delta is ms
			}

			if ((i == 0) && (tmpBoundaries.at(0) > op.getOriginalF0_Onset()))
			{
				tmpBoundaries.at(0) = op.getOriginalF0_Onset();
			}
		}
		std::sort(tmpBoundaries.begin(), tmpBoundaries.end());
		tmpBoundaries.back() = op.getOriginalF0_Offset();
		op.setBoundaries(tmpBoundaries);
		optBoundaries = tmpBoundaries;
	}
	for (unsigned i = 0; i < number_Targets; ++i)
	{
		PitchTarget pt;
		pt.slope = xtmp(number_optVar * i + 0);
		pt.offset = xtmp(number_optVar * i + 1);
		pt.tau = xtmp(number_optVar * i + 2);
		pt.duration = optBoundaries.at(i + 1) - optBoundaries.at(i);
		optTargets.push_back(pt);
	}


	// store optimum
	//op.setOptimum( optBoundaries, optTargets );
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	double computationTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	op.setOptimum(optBoundaries, optTargets, computationTime, ftmp_vector);
	std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;


		// DEBUG message
#ifdef DEBUG_MSG
	std::cout << "\t[optimize] mse = " << fmin << std::endl;
#endif
}



double BobyqaOptimizer::getRandomValue(const double min, const double max)
{
	return min + ((double)rand() / RAND_MAX) * (max - min);
}
