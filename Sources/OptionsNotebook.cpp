#ifdef USE_WXWIDGETS
#include "OptionsNotebook.h"
#include <iostream> //exclude again

OptimizationOptions OptionsNotebook::getOptions()
{
	ParameterSet params;
	auto searchParams = searchSpacePage->getParameters();
	auto regularizationParams = regularizationPage->getParameters();

	params.deltaOffset = searchParams.offsetDelta;
	params.deltaSlope = searchParams.slopeDelta;
	params.deltaTau = searchParams.tauDelta;
	params.deltaBoundary = searchParams.boundaryDelta;
	params.initBounds = searchParams.initBounds;
	params.lambda = regularizationParams.lambda;
	params.weightOffset = regularizationParams.weightOffset;
	params.weightSlope = regularizationParams.weightSlope;
	params.weightTau = regularizationParams.weightTau;
	params.meanSlope = 0.0;
	// TODO: Make meanTau a tunable parameter in the GUI (?)
	params.meanTau = 15.0;

	params.optimizeBoundaries = (searchParams.boundaryDelta != 0);
	if (params.optimizeBoundaries)
	{
		params.numberOptVar = 4;
	}
	else
	{
		params.numberOptVar = 3;
	}
	std::cout << "number of opt var: " << params.numberOptVar << std::endl;
	
	// TODO: Find a better place for meanF0 calculation
	//calculate mean f0
	params.meanOffset = 0.0;
	for (unsigned i = 0; i < Data::getInstance().originalF0.size(); ++i)
	{
		params.meanOffset += Data::getInstance().originalF0[i].value;
	}
	params.meanOffset /= Data::getInstance().originalF0.size();

	OptimizerOptions opts = optimizerPage->getParameters();
	
	return OptimizationOptions{ params, opts };
}

#endif // USE_WXWIDGETS