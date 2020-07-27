#pragma once
#ifdef USE_WXWIDGETS
#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include "wxPlotRegion.h"
#include "../Data.h"
#include "../OptimizationProblem.h"

class wxMainWindow : public wxFrame
{
public:
    wxMainWindow(const wxString& title = wxT("Target Optimizer"), 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxSize(1280, 720));
private:
    void OnAbout(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnOpenTextGrid(wxCommandEvent& event);
    void OnOpenPitchTier(wxCommandEvent& event);    
    //void OnAbout(wxCommandEvent& event);
    void updateWidgets();

private:
    //TODO: Replace flags with proper progress manager
    bool isAnyDataLoaded{ false };
    bool isAllDataLoaded{ false };
    bool isOptimized{ false };

    // The picture area containing the plots ("figure" in MATLAB parlance)
    wxPlotRegion* plotRegion { 
        new wxPlotRegion(this, 
        Data::getInstance().syllableBoundaries,
        Data::getInstance().originalF0
        )};

    ParameterSet collectParameters();
    wxDECLARE_EVENT_TABLE();
};
#endif // USE_WXWIDGETS