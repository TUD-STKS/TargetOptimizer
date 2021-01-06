#ifdef USE_WXWIDGETS
#include "OptionsNotebook.h"
#include <iostream> //exclude again

OptimizationOptions OptionsNotebook::getOptions()
{
	ParameterSet params;
	params.searchSpaceParameters = searchSpacePage->getParameters();
	params.regularizationParameters = regularizationPage->getParameters();
		

	// TODO: Find a better place for meanF0 calculation
	//calculate mean f0
	params.searchSpaceParameters.meanOffset = 0.0;
	for (unsigned i = 0; i < Data::getInstance().originalF0.size(); ++i)
	{
		params.searchSpaceParameters.meanOffset += Data::getInstance().originalF0[i].value;
	}
	params.searchSpaceParameters.meanOffset /= Data::getInstance().originalF0.size();

	OptimizerOptions opts = optimizerPage->getParameters();
	
	return OptimizationOptions{ params, opts };
}

void OptionsNotebook::setOptions(OptimizationOptions newOptions)
{
	regularizationPage->setParameters(newOptions.problemParams.regularizationParameters);
	searchSpacePage->setParameters(newOptions.problemParams.searchSpaceParameters);
	optimizerPage->setParameters(newOptions.optimizerOptions);
}

#endif // USE_WXWIDGETS