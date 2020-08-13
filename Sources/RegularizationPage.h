#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
#include "OptimizationProblem.h"

class RegularizationPage :
	public wxPanel
{
public:
	RegularizationPage(wxWindow *parent, wxWindowID id);
	RegularizationParameters getParameters();
private:
	wxSpinCtrlDouble* lambda;
	wxSpinCtrlDouble* weightSlope;
	wxSpinCtrlDouble* weightOffset;
	wxSpinCtrlDouble* weightTau;
};

#endif