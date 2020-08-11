#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>

struct OptimizerOptions 
{
	int maxIterations{ 100 };
	double correlationThreshold{ 0.99 };
	bool useCorrelationThreshold{ true };
	double rmseThreshold{ 0.1 };
	bool useRmseThreshold{ true };
};

class OptimizerPage :
	public wxPanel
{
public:
	OptimizerPage(wxWindow *parent, wxWindowID id);
	OptimizerOptions getParameters();
private:
	wxSpinCtrl* maxIterations;
	wxSpinCtrlDouble* correlationThreshold;
	wxCheckBox* useCorrelationThreshold;
	wxSpinCtrlDouble* rmseThreshold;
	wxCheckBox* useRmseThreshold;
};

#endif