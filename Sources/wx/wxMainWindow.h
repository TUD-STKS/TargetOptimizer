#pragma once
#ifdef USE_WXWIDGETS
#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/grid.h>
#include "wxPlotRegion.h"
#include "OptionsNotebook.h"
#include "../Data.h"
#include "../OptimizationProblem.h"


class wxMainWindow : public wxFrame
{
public:
    wxMainWindow(const wxString& title = wxT("Target Optimizer"), 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxSize(1280, 720));
private:
    void clear();
    void OnAbout(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnOpenTextGrid(wxCommandEvent& event);
    void OnOpenPitchTier(wxCommandEvent& event);    
    void OnOptimize(wxCommandEvent& event);
    void OnSaveAsGesture(wxCommandEvent& event);
    void OnSaveAsCsv(wxCommandEvent& event);
    void OnSaveAsPitchTier(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void updateWidgets();

private:
    //TODO: Replace flags with proper progress manager
    bool isTextGridLoaded{ false };
    bool isPitchTierLoaded{ false };
    bool isOptimized{ false };

    // The picture area containing the plots ("figure" in MATLAB parlance)
    wxPlotRegion* plotRegion{
        new wxPlotRegion(this,
        Data::getInstance().syllableBoundaries,
        Data::getInstance().originalF0,
        Data::getInstance().pitchTargets,
        Data::getInstance().optimalF0
        )};
    OptionsNotebook *optimizationOptions;
    wxGrid *resultsTable;

    wxDECLARE_EVENT_TABLE();
};
#endif // USE_WXWIDGETS