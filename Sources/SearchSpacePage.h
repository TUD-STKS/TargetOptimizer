#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "OptimizationProblem.h"


class SearchSpacePage :
	public wxPanel
{
public:
	SearchSpacePage(wxWindow *parent, wxWindowID id);
	
	SearchSpaceParameters getParameters();
	void setParameters(SearchSpaceParameters newParams);
private:
	wxSpinCtrlDouble *deltaSlopeCtrl;
	wxSpinCtrlDouble *deltaOffsetCtrl;
	wxSpinCtrlDouble *meanTauCtrl;
	wxSpinCtrlDouble *deltaTauCtrl;
	wxSpinCtrlDouble *deltaBoundaryCtrl;
	wxSpinCtrl       *initBoundsCtrl;

};

#endif