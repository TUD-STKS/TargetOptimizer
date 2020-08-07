#pragma once
#ifdef USE_WXWIDGETS
#include <wx/notebook.h>
#include "BoundaryPage.h"
#include "ResultPage.h"
#include "OptimizationProblem.h"


class TargetsNotebook :
	public wxNotebook
{
public:
    // Use a variadic template constructor to avoid re-implementing all the base class ctors
    template<typename... Args>
    TargetsNotebook(Args&&... args);

public:
	ParameterSet getOptions();
	void ResetTargetTables();
	BoundaryPage *boundaryPage;
    ResultPage *resultPage;

private:
	//TODO: Create an OptionsPage class
//    BoundaryPage *boundaryPage;
//    ResultPage* resultPage;
};

// Because it is templated, the constructor has to be in the header file
template<typename ...Args>
inline TargetsNotebook::TargetsNotebook(Args&& ...args) : wxNotebook(std::forward<Args>(args)...)
{
	// The first page of the notebook with search space options
	boundaryPage = new BoundaryPage(this, wxID_ANY);
	AddPage( boundaryPage, wxT("Target Boundaries"));
	// The second page of the notebook with regularization options
	resultPage = new ResultPage(this, wxID_ANY);
	AddPage(resultPage, wxT("Optimization Results"));
}

#endif // USE_WXWIDGETS