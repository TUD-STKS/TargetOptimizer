#include <iostream>
#include <string>
#include <dlib/cmd_line_parser.h>
#include "gui.h"
#include "BobyqaOptimizer.h"
#include "TextGridReader.h"

#ifdef USE_WXWIDGETS
#include <wx/wx.h>

class TargetOptimizerGui : public wxApp
{
public:
	virtual bool OnInit();

public:
	wxMainWindow *mainWindow;
};

wxIMPLEMENT_APP_NO_MAIN(TargetOptimizerGui);

bool TargetOptimizerGui::OnInit()
{
	mainWindow = new wxMainWindow();
	mainWindow->Show();
	return true;
}

#endif // USE_WXWIDGETS

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		try
		{
#ifdef USE_WXWIDGETS
			// ********** open wxWidgets GUI *****
			auto* targetOptimizerGui = new TargetOptimizerGui();
			wxApp::SetInstance(targetOptimizerGui);
			wxEntry(argc, argv);
#else
			// ********** open DLIB GUI **********
			// create window
			MainWindow myWindow;
			// tell window to put itself on the screen
			myWindow.show();

			// wait until the user closes this window before the program terminates
			myWindow.wait_until_closed();
#endif // USE_WXWIDGETS
		}
		catch (std::exception& e)
		{
			std::ostringstream msg;
			msg << "[main] Program was terminated because an error occurred!\n" << e.what() << std::endl;
			//message_box("Error", msg.str());
			return EXIT_FAILURE;
		}
	}
	else
	{
		try
		{
			//// ********** command line parsing **********
			//dlib::command_line_parser parser;

			//// command line options
			//parser.add_option("h","Display this help message.");
			//parser.set_group_name("Output Options");
			//parser.add_option("g","Choose for VTL gesture file.");
			//parser.add_option("c","Choose for csv table file.");
			//parser.add_option("p","Choose for PitchTier file.");
			//parser.set_group_name("Additional Parameter Options");
			//parser.add_option("lambda","Specify regularization parameter.",1);
			//parser.add_option("m-range","Specify search space for slope parameter.",1);
			//parser.add_option("b-range","Specify search space for offset parameter.",1);
			//parser.add_option("t-range","Specify search space for time constant parameter.",1);
			//parser.add_option("m-weight","Specify regularization weight for slope parameter.",1);
			//parser.add_option("b-weight","Specify regularization weight for offset parameter.",1);
			//parser.add_option("t-weight","Specify regularization weight for time constant parameter.",1);

			//// parse command line
			//parser.parse(argc,argv);

			//// check command line options
			//const char* one_time_opts[] = {"h", "g", "c", "p", "m-range", "b-range", "t-range", "m-weight", "b-weight", "t-weight"};
			//parser.check_one_time_options(one_time_opts);
			//parser.check_option_arg_range("m-range", 0.0, 100.0);
			//parser.check_option_arg_range("b-range", 0.0, 100.0);
			//parser.check_option_arg_range("t-range", 0.0, 14.999);
			//parser.check_option_arg_range("m-weight", 0.0, 1e9);
			//parser.check_option_arg_range("b-weight", 0.0, 1e9);
			//parser.check_option_arg_range("t-weight", 0.0, 1e9);
			//parser.check_option_arg_range("lambda", 0.0, 1e15);

			//// process help option
			//if (parser.option("h"))
			//{
			//	std::cout << "Usage: TargetOptimizer <TextGrid-file> <PitchTier-file> { <options> | <arg> }\n";
			//	parser.print_options();
			//	return EXIT_SUCCESS;
			//}

			//// check number of default arguments
			//if (parser.number_of_arguments() != 2)
			//{
			//	std::cout << "Error in command line:\n   You must specify two input files.\n";
			//	std::cout << "\nTry the -h option for more information." << std::endl;
			//	return EXIT_FAILURE;
			//}

			//// process TextGrid input
			//TextGridReader tgreader (parser[0]);
			//BoundaryVector bounds = tgreader.getBounds();

			//// process PitchTier input
			//PitchTierReader ptreader (parser[1]);
			//TimeSignal f0 = ptreader.getF0();
			//std::string fileName = ptreader.getFileName();

			////calculate mean f0
			//double meanF0 = 0.0;
			//for (std::vector<Sample>::const_iterator sp = f0.begin(); sp != f0.end(); ++sp)
			//{
			//	meanF0 += sp->value;
			//}
			//meanF0 /= f0.size();

			//// process optional parameter options
			//ParameterSet parameters;
			//parameters.deltaSlope = get_option(parser,"m-range",50.0);
			//parameters.deltaOffset = get_option(parser,"b-range",20.0);
			//parameters.deltaTau = get_option(parser,"t-range",5.0);
			//parameters.weightSlope = get_option(parser,"m-weight",1.0);
			//parameters.weightOffset = get_option(parser,"b-weight",0.5);
			//parameters.weightTau = get_option(parser,"t-weight",0.1);
			//parameters.lambda = get_option(parser,"lambda",0.01);
			//parameters.meanSlope = 0.0;
			//parameters.meanOffset = meanF0;
			//parameters.meanTau = 15.0;

			//// main functionality
			//OptimizationProblem problem (parameters, f0, bounds);
			//BobyqaOptimizer optimizer;
			//optimizer.optimize(problem);
			//TargetVector optTargets = problem.getPitchTargets();
			//TimeSignal optF0 = problem.getModelF0();
			//Sample onset = problem.getOnset();

			//// process gesture-file output option
			//if (parser.option("g"))
			//{
			//	GestureWriter gwriter (fileName + ".ges");
			//	gwriter.writeTargets(onset, optTargets);
			//}

			//// process csv-file output option
			//if (parser.option("c"))
			//{
			//	CsvWriter cwriter (fileName + ".csv");
			//	cwriter.writeTargets(onset, optTargets);
			//}

			//// process PitchTarget-file output option
			//if (parser.option("p"))
			//{
			//	PitchTierWriter pwriter (fileName + "-tam.PitchTier");
			//	pwriter.writeF0(optF0);
			//}

			//// print results
			//std::cout << "Optimization successful.\tRMSE=" << problem.getRootMeanSquareError() << "\tCORR=" << problem.getCorrelationCoefficient() << std::endl;

			//return EXIT_SUCCESS;
		}

		catch (std::exception& e)
		{
			std::cerr << "[main] Program was terminated because an error occurred!\n" << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
