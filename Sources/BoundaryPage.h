#pragma once
#ifdef USE_WXWIDGETS

#include <wx/wx.h>
//#include <wx/spinctrl.h>
#include <wx/grid.h>

struct BoundaryParameters
{
	double slopeDelta{ 50 };
	double offsetDelta{ 20 };
	double tauDelta{ 15 };
	double boundaryDelta{ 0.0 };
};

class BoundaryPage :
	public wxPanel
{
public:
	BoundaryPage(wxWindow *parent, wxWindowID id);
	
	BoundaryParameters getParameters();
private:
	//wxSpinCtrlDouble *slopeDeltaCtrl;
	//wxSpinCtrlDouble *offsetDeltaCtrl;
	//wxSpinCtrlDouble *tauDeltaCtrl;
	//wxSpinCtrlDouble *boundaryDeltaCtrl;

	wxGrid *boundaryTable;
};

#endif