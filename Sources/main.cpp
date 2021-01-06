#include <iostream>
#include <string>
#include <dlib/cmd_line_parser.h>
#include "gui.h"
#include "dataio.h"
#include "BobyqaOptimizer.h"
#include "TextGridReader.h"

#include <fstream>

#ifdef USE_WXWIDGETS
#include <wx/wx.h>

#if defined(__GNUC__)
// The GCC considers the C++17 filesystem header experimental
#include <experimental/filesystem>
namespace std::filesystem = std::experimental::filesystem
#else
#include <filesystem>
#endif

class TargetOptimizerGui : public wxApp
{
public:
	virtual bool OnInit();

public:
	MainWindow* mainWindow;
};

wxIMPLEMENT_APP_NO_MAIN(TargetOptimizerGui);

bool TargetOptimizerGui::OnInit()
{
	mainWindow = new MainWindow();
	mainWindow->Show();
	return true;
}

#endif // USE_WXWIDGETS

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
#ifdef USE_WXWIDGETS
		// If using the GUI redirect console output to log file
		std::cout << "When using the graphical user interface, all output to stdout is re-routed to a log file 'TargetOptimizer.log'."
			<< "Look for it in " << std::filesystem::current_path().string() << std::endl;
		std::ofstream outFile("TargetOptimizer.log");
		std::cout.rdbuf(outFile.rdbuf());

		// ********** open wxWidgets GUI *****
		auto* targetOptimizerGui = new TargetOptimizerGui();
		wxApp::SetInstance(targetOptimizerGui);
		wxEntry(argc, argv);

#else
		std::cerr << "TargetOptimizer was compiled without GUI support.\n"
			<< "To learn how to use it as a command line tool, use 'TargetOptimizer -h'\n"
			<< "To use the GUI version, recompile with the USE_WXWIDGETS preprocessor macro (requires wxWidgets)" << std::endl;
#endif // USE_WXWIDGETS		
	}
	else
	{
		try
		{
			// ********** command line parsing **********
			dlib::command_line_parser parser;

			// command line options
			parser.add_option("h", "Display this help message.");
			parser.set_group_name("Output Options");
			parser.add_option("g", "Choose for VTL gesture file.");
			parser.add_option("c", "Choose for csv table file.");
			parser.add_option("p", "Choose for PitchTier file.");
			parser.set_group_name("Additional Parameter Options");
			parser.add_option("tg", "Specify a TextGrid file.", 1);
			parser.add_option("lambda", "Specify regularization parameter.", 1);
			parser.add_option("m-range", "Specify search space for slope parameter.", 1);
			parser.add_option("b-range", "Specify search space for offset parameter.", 1);
			parser.add_option("t-range", "Specify search space for time constant parameter.", 1);
			parser.add_option("m-weight", "Specify regularization weight for slope parameter.", 1);
			parser.add_option("b-weight", "Specify regularization weight for offset parameter.", 1);
			parser.add_option("t-weight", "Specify regularization weight for time constant parameter.", 1);
			parser.add_option("boundaryDelta", "Specify boundary delta.", 1);
			parser.add_option("initBounds", "Specify how many bounds should be initialized.", 1);
			parser.add_option("maxIterations", "Specify maximum number of optimizer iterations.", 1);
			parser.add_option("maxCostEvaluations", "Specify maximum number of costfunction evaluations.", 1);
			parser.add_option("rhoEnd", "Specify the rho_end parameter.", 1);
			parser.add_option("epsilon", "Specify the epsilon for early stopping", 1);
			parser.add_option("patience", "Specify the patience for early stopping", 1);
			parser.add_option("log", "Outputs f0, targets, boundaries, etc. to a single text file.");
			parser.add_option("useEarlyStopping", "Stop the search early if fmin-ftmp < epsilon for x times (x=patience).");
			parser.add_option("utterance", "Name of the utterance to optimize.", 1);


			// parse command line
			parser.parse(argc, argv);

			// check command line options
			const char* one_time_opts[] = { "h", "g", "c", "p", "m-range", "b-range", "t-range", "m-weight", "b-weight", "t-weight" };
			parser.check_one_time_options(one_time_opts);
			parser.check_option_arg_range("m-range", 0.0, 100.0);
			parser.check_option_arg_range("b-range", 0.0, 100.0);
			parser.check_option_arg_range("t-range", 0.0, 14.999);
			parser.check_option_arg_range("m-weight", 0.0, 1e9);
			parser.check_option_arg_range("b-weight", 0.0, 1e9);
			parser.check_option_arg_range("t-weight", 0.0, 1e9);
			parser.check_option_arg_range("lambda", 0.0, 1e15);
			parser.check_option_arg_range("boundaryDelta", 0.0, 100.0);
			parser.check_option_arg_range("maxIterations", 1, 20000);

			// process help option
			if (parser.option("h"))
			{
				std::cout << "Usage: TargetOptimizer <PitchTier-file> <LOG-name> { <options> | <arg> }\n";
				parser.print_options();
				return EXIT_SUCCESS;
			}

			// check number of default arguments
			if (parser.number_of_arguments() != 2)
			{
				std::cout << "Error in command line:\n   You must specify a PitchTier file and the LOG name.\n";
				std::cout << "\nTry the -h option for more information." << std::endl;
				return EXIT_FAILURE;
			}

			// process optional parameter options
			ParameterSet parameters;
			parameters.searchSpaceParameters.initBounds = get_option(parser, "initBounds", 0);

			BoundaryVector bounds;
			std::string TextGrid = get_option(parser, "tg", "");
			if (TextGrid != "")
			{
				// process TextGrid input
				bounds = DataIO::readTextGridFile(TextGrid).getBounds();
			}
			else if (parameters.searchSpaceParameters.initBounds < 2)
			{
				std::cout << "You need at least two boundaries! Specify a TextGrid file or set initBounds >=2." << std::endl;
				return EXIT_FAILURE;
			}


			// process PitchTier input
			auto ptreader = DataIO::readPitchTierFile(parser[0]);
			TimeSignal f0 = ptreader.getF0();
			std::string fileName = ptreader.getFileName();


			std::string LOG_PATH = parser[1];
			std::string UTTERANCE = get_option(parser, "utterance", "default_str");

			std::cout << "log path: " << LOG_PATH << std::endl;


			//calculate mean f0
			double meanF0 = 0.0;
			for (std::vector<Sample>::const_iterator sp = f0.begin(); sp != f0.end(); ++sp)
			{
				meanF0 += sp->value;
			}
			meanF0 /= f0.size();




			parameters.regularizationParameters.weightSlope = get_option(parser, "m-weight", 1.0);
			parameters.regularizationParameters.weightOffset = get_option(parser, "b-weight", 0.5);
			parameters.regularizationParameters.weightTau = get_option(parser, "t-weight", 0.1);
			parameters.regularizationParameters.lambda = get_option(parser, "lambda", 0.01);

			parameters.searchSpaceParameters.deltaSlope = get_option(parser, "m-range", 50.0);
			parameters.searchSpaceParameters.deltaOffset = get_option(parser, "b-range", 20.0);
			parameters.searchSpaceParameters.deltaTau = get_option(parser, "t-range", 5.0);
			parameters.searchSpaceParameters.meanSlope = 0.0;
			parameters.searchSpaceParameters.meanOffset = meanF0;
			parameters.searchSpaceParameters.meanTau = 20.0;
			parameters.searchSpaceParameters.deltaBoundary = get_option(parser, "boundaryDelta", 100.0);

			parameters.searchSpaceParameters.optimizeBoundaries = (parameters.searchSpaceParameters.deltaBoundary != 0);
			parameters.searchSpaceParameters.numberOptVar = (parameters.searchSpaceParameters.optimizeBoundaries ? 4 : 3);

			OptimizerOptions optOpt;
			optOpt.maxIterations = get_option(parser, "maxIterations", 50.0);
			if (parser.option("useEarlyStopping"))
			{
				optOpt.useEarlyStopping = true;
			}
			else {
				optOpt.useEarlyStopping = false;
			}
			optOpt.epsilon = get_option(parser, "epsilon", 0.01);
			optOpt.patience = get_option(parser, "patience", 10);
			optOpt.maxCostEvaluations = get_option(parser, "maxCostEvaluations", 1e6);
			optOpt.rhoEnd = get_option(parser, "rhoEnd", 1e-6);





			if (parameters.searchSpaceParameters.initBounds != 0)
			{
				std::cout << "bounds init" << std::endl;
				double pitch_start = f0.at(0).time;
				double pitch_end = f0.back().time;
				double pitch_interval = pitch_end - pitch_start;
				double step = pitch_interval / (parameters.searchSpaceParameters.initBounds - 1);


				std::vector<double> initBoundaries;
				for (int i = 0; i < parameters.searchSpaceParameters.initBounds; ++i)
				{
					initBoundaries.push_back(pitch_start + i * step);
				}
				bounds = initBoundaries;
				initBoundaries.clear();
			}

			// main functionality
			OptimizationProblem problem(parameters, f0, bounds);
			BobyqaOptimizer optimizer;
			optimizer.optimize(problem, optOpt);
			TargetVector optTargets = problem.getPitchTargets();
			BoundaryVector optBoundaries = problem.getBoundaries();
			TimeSignal optF0 = problem.getModelF0();
			Sample onset = problem.getOnset();
			std::vector<double> ftmp_vector = problem.getOptimizationSolutions();

			if (parser.option("log"))
			{
				std::ofstream LOG;
				LOG.open(LOG_PATH);
				LOG << "utterance, ";
				LOG << "par_s_slope_delta, ";
				LOG << "par_s_offset_delta, ";
				LOG << "par_s_tau_mean, ";
				LOG << "par_s_tau_delta, ";
				LOG << "par_s_boundary_delta, ";
				LOG << "par_s_initbounds, ";
				LOG << "par_r_lambda, ";
				LOG << "par_r_slope_weight, ";
				LOG << "par_r_offset_weight, ";
				LOG << "par_r_tau_weight, ";
				LOG << "par_o_maxiter, ";
				LOG << "par_o_maxcost, ";
				LOG << "par_o_rhoend, ";
				LOG << "par_o_earlystop, ";
				LOG << "par_o_epsilon, ";
				LOG << "par_o_patience, ";
				LOG << "ini_f0_time_array, ";
				LOG << "ini_f0_value_array, ";
				LOG << "ini_boundary_array, ";
				LOG << "res_fmin, ";
				LOG << "res_rmse, ";
				LOG << "res_corr, ";
				LOG << "res_time, ";
				LOG << "res_ftmp_array, ";
				LOG << "res_f0_time_array, ";
				LOG << "res_f0_value_array, ";
				LOG << "res_boundary_array, ";
				LOG << "res_t_slope_array, ";
				LOG << "res_t_offset_array, ";
				LOG << "res_t_tau_array, ";
				LOG << "res_t_duration_array ";
				LOG << "\n";

				LOG << UTTERANCE << ", ";
				LOG << parameters.searchSpaceParameters.deltaSlope << ", ";
				LOG << parameters.searchSpaceParameters.deltaOffset << ", ";
				LOG << parameters.searchSpaceParameters.meanTau << ", ";
				LOG << parameters.searchSpaceParameters.deltaTau << ", ";
				LOG << parameters.searchSpaceParameters.deltaBoundary << ", ";
				LOG << parameters.searchSpaceParameters.initBounds << ", ";
				LOG << parameters.regularizationParameters.lambda << ", ";
				LOG << parameters.regularizationParameters.weightSlope << ", ";
				LOG << parameters.regularizationParameters.weightOffset << ", ";
				LOG << parameters.regularizationParameters.weightTau << ", ";
				LOG << optOpt.maxIterations << ", ";
				LOG << optOpt.maxCostEvaluations << ", ";
				LOG << optOpt.rhoEnd << ", ";
				LOG << optOpt.useEarlyStopping << ", ";
				LOG << optOpt.epsilon << ", ";
				LOG << optOpt.patience << ", ";
				for (unsigned i = 0; i < f0.size(); ++i)
				{
					LOG << f0.at(i).time << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < f0.size(); ++i)
				{
					LOG << f0.at(i).value << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < bounds.size(); ++i)
				{
					LOG << bounds.at(i) << " ";
				}
				LOG << ", ";
				LOG << problem.getCostFunction() << ", ";
				LOG << problem.getRootMeanSquareError() << ", ";
				LOG << problem.getCorrelationCoefficient() << ", ";
				LOG << problem.getComputationTime() << ", ";
				for (unsigned i = 0; i < ftmp_vector.size(); ++i)
				{
					LOG << ftmp_vector.at(i) << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optF0.size(); ++i)
				{
					LOG << optF0.at(i).time << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optF0.size(); ++i)
				{
					LOG << optF0.at(i).value << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optBoundaries.size(); ++i)
				{
					LOG << optBoundaries.at(i) << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optTargets.size(); ++i)
				{
					LOG << optTargets.at(i).slope << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optTargets.size(); ++i)
				{
					LOG << optTargets.at(i).offset << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optTargets.size(); ++i)
				{
					LOG << optTargets.at(i).tau << " ";
				}
				LOG << ", ";
				for (unsigned i = 0; i < optTargets.size(); ++i)
				{
					LOG << optTargets.at(i).duration << " ";
				}
				LOG << "\n";
				LOG.close();
			}

			// process gesture-file output option
			if (parser.option("g"))
			{
				DataIO::saveGesturalScore(onset, optTargets, fileName + ".ges");
			}

			// process csv-file output option
			if (parser.option("c"))
			{
				DataIO::saveCsvFile(onset, optTargets, fileName + ".csv");
			}

			// process PitchTarget-file output option
			if (parser.option("p"))
			{
				DataIO::savePitchTier(optF0, fileName + "-tam.PitchTier");
			}

			// print results
			std::cout << "Optimization successful.\tRMSE=" << problem.getRootMeanSquareError() << "\tCORR=" << problem.getCorrelationCoefficient() << std::endl;

			return EXIT_SUCCESS;
		}

		catch (std::exception & e)
		{
			std::cerr << "[main] Program was terminated because an error occurred!\n" << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
