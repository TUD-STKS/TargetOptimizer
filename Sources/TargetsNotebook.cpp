#ifdef USE_WXWIDGETS
#include "TargetsNotebook.h"

ParameterSet TargetsNotebook::getOptions()
{
	ParameterSet params;
	//auto boundaryParams = boundaryPage->getParameters();
//	auto resultParams = resultPage->getParameters();

	//params.deltaOffset = searchParams.offsetDelta;
	//params.deltaSlope = searchParams.slopeDelta;
	//params.deltaTau = searchParams.tauDelta;
	//params.deltaBoundary = searchParams.boundaryDelta;
	//params.lambda = regularizationParams.lambda;
	//params.weightOffset = regularizationParams.weightOffset;
	//params.weightSlope = regularizationParams.weightSlope;
	//params.weightTau = regularizationParams.weightTau;
	//params.meanSlope = 0.0;
	//// TODO: Make meanTau a tunable parameter in the GUI (?)
	//params.meanTau = 15.0;
	//// TODO: Find a better place for meanF0 calculation
	////calculate mean f0
	//params.meanOffset = 0.0;
	//for (unsigned i = 0; i < Data::getInstance().originalF0.size(); ++i)
	//{
	//	params.meanOffset += Data::getInstance().originalF0[i].value;
	//}
	//params.meanOffset /= Data::getInstance().originalF0.size();


	return params;
}

#endif // USE_WXWIDGETS