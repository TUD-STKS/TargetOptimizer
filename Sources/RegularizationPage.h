#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>

struct RegularizationParameters 
{
	double lambda{ 0.0 };
	double weightSlope{ 1.0 };
	double weightOffset{ 0.5 };
	double weightTau{ 0.1 };
};

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