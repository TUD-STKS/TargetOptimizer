#pragma once
#include <dlib/gui_widgets.h>
#include "dataio.h"
#include "PlotRegion.h"
#include "BobyqaOptimizer.h"

using namespace dlib;

class MainWindow : public drawable_window
{
public:
    MainWindow();
    ~MainWindow();

private:

    // Private helper methods
    void noOptimizationPerformed();
    ParameterSet readParameters();

    // Event handlers
    void onButtonClear();
    void onButtonTextGridOpen();
    void onButtonPitchTierOpen();
    void onButtonOptimize();
    void openTextGrid(const std::string& fileName);
    void openPitchTier(const std::string& fileName);
    void onReadyForOptimize();
    void onSaveFileGesture(const std::string& fileName);
    void onButtonSaveAsGesture();
    void onSaveFileCsv(const std::string& fileName);
    void onMenuSaveAsCsv();
    void onSaveFilePitchTier(const std::string& fileName);
    void onMenuSaveAsPitchTier();
    void unblockMainWindow();
    void onMenuFileQuit();
    void onMenuFileHelp();
    void onMenuFileAbout();
    void on_window_resized();
    void update();

    // GUI widgets
    PlotRegion graph;
    button btnLoadTextGrid;
    button btnLoadPitchTier;
    button btnOptimize;
    button btnStoreGesture;
    button btnStoreCsv;
    button btnClear;
    menu_bar mbar;
    named_rectangle recTargets;
    named_rectangle recOptions;
    named_rectangle recActions;
    text_grid targetGrid;
    widget_group searchSpaceGroup;
    widget_group penaltyGroup;
    tabbed_display tabs;
    label lbSlope, lbOffset, lbTau, lbLambda, lbWeightm, lbWeightb, lbWeighttau;
    text_field tfSlope, tfOffset, tfTau, tfLambda, tfWeightm, tfWeightb, tfWeighttau;
};