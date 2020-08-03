#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
#include <wx/spinctrl.h>


struct SearchSpaceParameters
{
	double slopeDelta{ 50 };
	double offsetDelta{ 20 };
	double tauDelta{ 15 };
	double boundaryDelta{ 5 };
};

class SearchSpacePage :
	public wxPanel
{
public:
	SearchSpacePage(wxWindow *parent, wxWindowID id);
	
	SearchSpaceParameters getParameters();
private:
	wxSpinCtrlDouble *slopeDeltaCtrl;
	wxSpinCtrlDouble *offsetDeltaCtrl;
	wxSpinCtrlDouble *tauDeltaCtrl;
	wxSpinCtrlDouble *boundaryDeltaCtrl;

};

#endif