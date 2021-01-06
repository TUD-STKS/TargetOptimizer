#ifdef USE_WXWIDGETS
#include "TargetsNotebook.h"

ParameterSet TargetsNotebook::getOptions()
{
	ParameterSet params;
	return params;
}

void TargetsNotebook::ResetTargetTables()
{
	boundaryPage->boundaryTable->DeleteCols(0, boundaryPage->boundaryTable->GetNumberCols());
	boundaryPage->boundaryTable->InsertCols(0, 1);
	boundaryPage->boundaryTable->SetColLabelValue(0, wxEmptyString);
	
	resultPage->resultsTable->DeleteCols(0, resultPage->resultsTable->GetNumberCols());
	resultPage->resultsTable->InsertCols(0, 1);
	resultPage->resultsTable->SetColLabelValue(0, wxEmptyString);
}

#endif // USE_WXWIDGETS