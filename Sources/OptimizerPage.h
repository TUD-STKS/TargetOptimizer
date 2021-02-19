#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include "BobyqaOptimizer.h"


class OptimizerPage :
	public wxPanel
{
public:
	OptimizerPage(wxWindow* parent, wxWindowID id);
	OptimizerOptions getParameters() const;
	void setParameters(OptimizerOptions newOptions);

private:
	void OnChangeValue(wxSpinEvent& event);
	void OnChangeValue(wxSpinDoubleEvent& event);

private:
	wxSpinCtrl* maxIterations;
	wxSpinCtrlDouble* rhoEnd;
	wxSpinCtrl* maxCostEvaluations;
	wxCheckBox* useEarlyStopping;
	wxSpinCtrlDouble* epsilon;	
	wxSpinCtrl* patience;

	wxDECLARE_EVENT_TABLE();
};

#endif