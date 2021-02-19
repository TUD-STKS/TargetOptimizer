#include <fstream>
#include <iomanip>
#include "TOWriter.h"


void TOWriter::writeLOG(const Sample& onset, const TargetVector& optTargets, const BoundaryVector& optBoundaries) const
{
	//// create output file and write results to it
	//std::ofstream LOG;
	//LOG.open(m_file.c_str());
	////		// main functionality
	////		OptimizationProblem problem (parameters, f0, bounds);
	////		BobyqaOptimizer optimizer;
	////		optimizer.optimize(problem, optOpt);
	////		TargetVector optTargets = problem.getPitchTargets();
	////		BoundaryVector optBoundaries = problem.getBoundaries();
	////		TimeSignal optF0 = problem.getModelF0();
	////		Sample onset = problem.getOnset();
	////		std::vector<double> ftmp_vector = problem.getOptimizationSolutions();
	//std::ofstream LOG;
	//LOG.open ( LOG_PATH );
	//LOG << "utterance, ";
	//LOG << "par_s_slope_delta, ";
	//LOG << "par_s_offset_delta, ";
	//LOG << "par_s_tau_mean, ";
	//LOG << "par_s_tau_delta, ";
	//LOG << "par_s_boundary_delta, ";
	//LOG << "par_s_initbounds, ";
	//LOG << "par_r_lambda, ";
	//LOG << "par_r_slope_weight, ";
	//LOG << "par_r_offset_weight, ";
	//LOG << "par_r_tau_weight, ";
	//LOG << "par_o_maxiter, ";
	//LOG << "par_o_maxcost, ";
	//LOG << "par_o_rhoend, ";
	//LOG << "par_o_earlystop, ";
	//LOG << "par_o_epsilon, ";
	//LOG << "par_o_patience, ";
	//LOG << "ini_f0_time_array, ";
	//LOG << "ini_f0_value_array, ";
	//LOG << "ini_boundary_array, ";
	//LOG << "res_fmin, ";
	//LOG << "res_rmse, ";
	//LOG << "res_corr, ";
	//LOG << "res_time, ";
	//LOG << "res_ftmp_array, ";
	//LOG << "res_f0_time_array, ";
	//LOG << "res_f0_value_array, ";
	//LOG << "res_boundary_array, ";
	//LOG << "res_t_slope_array, ";
	//LOG << "res_t_offset_array, ";
	//LOG << "res_t_tau_array, ";
	//LOG << "res_t_duration_array ";
	//LOG << "\n";
	//LOG << UTTERANCE << ", ";
	//LOG << parameters.searchSpaceParameters.deltaSlope << ", ";
	//LOG << parameters.searchSpaceParameters.deltaOffset << ", ";
	//LOG << parameters.searchSpaceParameters.meanTau << ", ";
	//LOG << parameters.searchSpaceParameters.deltaTau << ", ";
	//LOG << parameters.searchSpaceParameters.deltaBoundary << ", ";
	//LOG << parameters.searchSpaceParameters.initBounds << ", ";
	//LOG << parameters.regularizationParameters.lambda << ", ";
	//LOG << parameters.regularizationParameters.weightSlope << ", ";
	//LOG << parameters.regularizationParameters.weightOffset << ", ";
	//LOG << parameters.regularizationParameters.weightTau << ", ";
	//LOG << optOpt.maxIterations<< ", ";
	//LOG << optOpt.maxCostEvaluations << ", ";
	//LOG << optOpt.rhoEnd << ", ";
	//LOG << optOpt.useEarlyStopping << ", ";
	//LOG << optOpt.epsilon << ", ";
	//LOG << optOpt.patience << ", ";
	//for (unsigned i = 0; i < f0.size(); ++i)
	//{
	//	LOG << f0.at(i).time << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < f0.size(); ++i)
	//{
	//	LOG << f0.at(i).value << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < bounds.size(); ++i)
	//{
	//	LOG << bounds.at(i)<< " ";
	//}
	//LOG << ", ";
	//LOG << problem.getCostFunction() << ", ";
	//LOG << problem.getRootMeanSquareError() << ", ";
	//LOG << problem.getCorrelationCoefficient() << ", ";
	//LOG << problem.getComputationTime() << ", ";
	//for (unsigned i = 0; i < ftmp_vector.size(); ++i)
	//{
	//	LOG << ftmp_vector.at(i) << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optF0.size(); ++i)
	//{
	//	LOG << optF0.at(i).time << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optF0.size(); ++i)
	//{
	//	LOG << optF0.at(i).value << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optBoundaries.size(); ++i)
	//{
	//	LOG << optBoundaries.at(i)<< " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optTargets.size(); ++i)
	//{
	//	LOG << optTargets.at(i).slope << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optTargets.size(); ++i)
	//{
	//	LOG << optTargets.at(i).offset << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optTargets.size(); ++i)
	//{
	//	LOG << optTargets.at(i).tau << " ";
	//}
	//LOG << ", ";
	//for (unsigned i = 0; i < optTargets.size(); ++i)
	//{
	//	LOG << optTargets.at(i).duration << " ";
	//}
	//LOG << "\n";
	//LOG.close();
}