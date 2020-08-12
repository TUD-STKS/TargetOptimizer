#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>

struct OptimizerOptions 
{
	int maxIterations{ 10 };
	double correlationThreshold{ 0.99 };
	bool useCorrelationThreshold{ false };
	double rmseThreshold{ 0.2 };
	bool useRmseThreshold{ false };
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