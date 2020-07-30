#pragma once
#ifdef USE_WXWIDGETS
#include <wx/notebook.h>
#include "SearchSpacePage.h"
#include "RegularizationPage.h"
#include "OptimizationProblem.h"


class OptionsNotebook :
	public wxNotebook
{
public:
    // Use a variadic template constructor to avoid re-implementing all the base class ctors
    template<typename... Args>
    OptionsNotebook(Args&&... args);

public:
	ParameterSet getOptions();

private:
	//TODO: Create an OptionsPage class
    SearchSpacePage *searchSpacePage;
    RegularizationPage* regularizationPage;
};

// Because it is templated, the constructor has to be in the header file
template<typename ...Args>
inline OptionsNotebook::OptionsNotebook(Args&& ...args) : wxNotebook(std::forward<Args>(args)...)
{
	// The first page of the notebook with search space options
	searchSpacePage = new SearchSpacePage(this, wxID_ANY);
	AddPage(searchSpacePage, wxT("Search Space"));
	// The second page of the notebook with regularization options
	regularizationPage = new RegularizationPage(this, wxID_ANY);
	AddPage(regularizationPage, wxT("Regularization"));
}

#endif // USE_WXWIDGETS