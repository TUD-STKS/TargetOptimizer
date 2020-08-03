#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
//#include <wx/spinctrl.h>
#include <wx/grid.h>

struct ResultParameters
{
	double slopeDelta{ 50 };
	double offsetDelta{ 20 };
	double tauDelta{ 15 };
	double boundaryDelta{ 0.0 };
};

class ResultPage :
	public wxPanel
{
public:
	ResultPage(wxWindow *parent, wxWindowID id);
	
	ResultParameters getParameters();
private:
	//wxSpinCtrlDouble *slopeDeltaCtrl;
	//wxSpinCtrlDouble *offsetDeltaCtrl;
	//wxSpinCtrlDouble *tauDeltaCtrl;
	//wxSpinCtrlDouble *boundaryDeltaCtrl;

    wxGrid *resultsTable;
};

#endif