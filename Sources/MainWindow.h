#pragma once
#ifdef USE_WXWIDGETS
#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/grid.h>
#include "PlotRegion.h"
#include "OptionsNotebook.h"
#include "TargetsNotebook.h"
#include "Data.h"
#include "dataio.h"
#include "OptimizationProblem.h"


class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title = wxT("Target Optimizer"), 
        const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxSize(1280, 720));
private:
    void clear();
    void OnAbout(wxCommandEvent& event);
    void OnBoundaryCellChanged(wxGridEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnInitBounds(wxCommandEvent& event);   
    void OnOpen(wxCommandEvent& event);
    void OnOptimize(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void updateWidgets();


private:
    //TODO: Replace flags with proper progress manager
    bool isTextGridLoaded{ false };
    bool isPitchTierLoaded{ false };
    bool isOptimized{ false };

    // The picture area containing the plots ("figure" in MATLAB parlance)
    PlotRegion* plotRegion{
        new PlotRegion(this,
        Data::getInstance().syllableBoundaries,
        Data::getInstance().originalF0,
        Data::getInstance().pitchTargets,
        Data::getInstance().optimalF0
        )};
    OptionsNotebook *optimizationOptions;
    OptionsNotebook *optimizationOptions2;
    TargetsNotebook *targetOptions;
    TextGridReader tg;

    wxDECLARE_EVENT_TABLE();
};
#endif // USE_WXWIDGETS