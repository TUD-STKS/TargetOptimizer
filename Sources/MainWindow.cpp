#include "MainWindow.h"
MainWindow::MainWindow() :
    graph(*this, Data::getInstance().syllableBoundaries, Data::getInstance().pitchTargets,
        Data::getInstance().optimalF0, Data::getInstance().originalF0),
    btnLoadTextGrid(*this),
    btnLoadPitchTier(*this),
    btnOptimize(*this),
    btnStoreGesture(*this),
    btnStoreCsv(*this),
    btnClear(*this),
    mbar(*this),
    recTargets(*this),
    recOptions(*this),
    recActions(*this),
    targetGrid(*this),
    searchSpaceGroup(*this),
    penaltyGroup(*this),
    tabs(*this),
    lbSlope(*this), lbOffset(*this), lbTau(*this), lbLambda(*this), lbWeightm(*this), lbWeightb(*this), lbWeighttau(*this),
    tfSlope(*this), tfOffset(*this), tfTau(*this), tfLambda(*this), tfWeightm(*this), tfWeightb(*this), tfWeighttau(*this)
{
    set_title("Target Optimizer");

    // position the widget that is responsible for drawing the directed graph, the graph_drawer,
    // just below the mbar (menu bar) widget.
    graph.set_pos(15, mbar.bottom() + 5);
    graph.set_min_zoom_scale(1.0);
    set_size(750, 520);

    // register the event handlers with their respective widgets
    btnLoadTextGrid.set_click_handler(*this, &MainWindow::onButtonTextGridOpen);
    btnLoadPitchTier.set_click_handler(*this, &MainWindow::onButtonPitchTierOpen);
    btnOptimize.set_click_handler(*this, &MainWindow::onButtonOptimize);
    btnClear.set_click_handler(*this, &MainWindow::onButtonClear);
    btnStoreGesture.set_click_handler(*this, &MainWindow::onButtonSaveAsGesture);
    btnStoreCsv.set_click_handler(*this, &MainWindow::onMenuSaveAsCsv);

    // now set the text of some of our buttons and labels
    btnLoadTextGrid.set_name("Load TextGrid");
    btnLoadPitchTier.set_name("Load PitchTier");
    btnOptimize.set_name("Optimize");
    btnClear.set_name("Clear");
    btnStoreGesture.set_name("Save as Gesture");
    btnStoreCsv.set_name("Save as CSV");
    recTargets.set_name("Targets");
    recOptions.set_name("Options");
    recActions.set_name("Actions");

    // Now setup the tabbed display.  It will have two tabs, one for the search space
    // and for regularization
    tabs.set_number_of_tabs(2);
    tabs.set_tab_name(0, "Search Space");
    tabs.set_tab_name(1, "Regularization");
    searchSpaceGroup.add(lbSlope, 0, 12);
    searchSpaceGroup.add(lbOffset, 0, 42);
    searchSpaceGroup.add(lbTau, 0, 72);
    searchSpaceGroup.add(tfSlope, 130, 10);
    searchSpaceGroup.add(tfOffset, 130, 40);
    searchSpaceGroup.add(tfTau, 130, 70);
    penaltyGroup.add(lbLambda, 0, 12);
    penaltyGroup.add(lbWeightm, 0, 42);
    penaltyGroup.add(lbWeightb, 0, 72);
    penaltyGroup.add(lbWeighttau, 0, 102);
    penaltyGroup.add(tfLambda, 100, 10);
    penaltyGroup.add(tfWeightm, 100, 40);
    penaltyGroup.add(tfWeightb, 100, 70);
    penaltyGroup.add(tfWeighttau, 100, 100);
    tabs.set_tab_group(0, searchSpaceGroup);
    tabs.set_tab_group(1, penaltyGroup);

    // Now setup the menu bar.  We will have two menus.  A File and Help menu.
    mbar.set_number_of_menus(2);
    mbar.set_menu_name(0, "File", 'F');
    mbar.set_menu_name(1, "Help", 'H');

    // add the entries to the File menu.
    mbar.menu(0).add_menu_item(menu_item_text("Open TextGrid", *this, &MainWindow::onButtonTextGridOpen, 'T'));
    mbar.menu(0).add_menu_item(menu_item_text("Open PitchTier", *this, &MainWindow::onButtonPitchTierOpen, 'P'));
    mbar.menu(0).add_menu_item(menu_item_separator());
    mbar.menu(0).add_menu_item(menu_item_text("Optimize", *this, &MainWindow::onButtonOptimize, 'O'));
    mbar.menu(0).add_menu_item(menu_item_separator());
    mbar.menu(0).add_menu_item(menu_item_text("Save As Gesture", *this, &MainWindow::onButtonSaveAsGesture, 'G'));
    mbar.menu(0).add_menu_item(menu_item_text("Save As CSV", *this, &MainWindow::onMenuSaveAsCsv, 'C'));
    mbar.menu(0).add_menu_item(menu_item_text("Save As PitchTier", *this, &MainWindow::onMenuSaveAsPitchTier, 'i'));
    mbar.menu(0).add_menu_item(menu_item_separator());
    mbar.menu(0).add_menu_item(menu_item_text("Clear", *this, &MainWindow::onButtonClear, 'C'));
    mbar.menu(0).add_menu_item(menu_item_text("Quit", *this, &MainWindow::onMenuFileQuit, 'Q'));

    // Add the entries to the Help menu.
    mbar.menu(1).add_menu_item(menu_item_text("Help", *this, &MainWindow::onMenuFileHelp, 'H'));
    mbar.menu(1).add_menu_item(menu_item_text("About", *this, &MainWindow::onMenuFileAbout, 'A'));

    // call our helper functions and window resize event to get the widgets
    // to all arrange themselves correctly in our window.
    noOptimizationPerformed();
    on_window_resized();
}

MainWindow::~MainWindow()
{
    close_window();
}

void MainWindow::noOptimizationPerformed()
{
    btnOptimize.disable();
    btnStoreGesture.disable();
    targetGrid.disable();
    btnStoreCsv.disable();

    lbSlope.set_text("slope: 0.0 [st/s]     +/-");
    lbOffset.set_text("offset: f0-mean [st] +/-");
    lbTau.set_text("tau:    15.0 [ms]     +/-");
    lbLambda.set_text("lambda");
    lbWeightm.set_text("weight-slope");
    lbWeightb.set_text("weight-offset");
    lbWeighttau.set_text("weight-tau");
    tfSlope.set_text("50.0");
    tfOffset.set_text("20.0");
    tfTau.set_text("5.0");
    tfLambda.set_text("0.01");
    tfWeightm.set_text("1.0");
    tfWeightb.set_text("0.5");
    tfWeighttau.set_text("0.1");
}

void MainWindow::on_window_resized()
{
    // when you override any of the drawable_window events you have to make sure you
    // call the drawable_window's version of them because it needs to process
    // the events as well.  So we do that here.
    drawable_window::on_window_resized();

    // The rest of this function positions the widgets on the window
    unsigned long width, height;
    get_size(width, height);

    // Don't do anything if the user just made the window too small.  That is, leave
    // the widgets where they are.
    if (width < 500 || height < 350)
        return;

    // Set the size of the probability tables and the drawing area for the graph
    graph.set_size(0.97 * width, 0.55 * height - +mbar.height());
    targetGrid.set_size(width - (recOptions.width() + recActions.width() + 0.125 * width), 174);
    // tell the tabbed display to make itself just the right size to contain
    // the two probability tables.
    tabs.fit_to_contents();

    tfSlope.set_width(60);
    tfOffset.set_width(60);
    tfTau.set_width(60);
    tfLambda.set_width(60);
    tfWeightm.set_width(60);
    tfWeightb.set_width(60);
    tfWeighttau.set_width(60);

    btnLoadTextGrid.set_pos(255, graph.bottom() + 30);
    btnLoadPitchTier.set_pos(255, graph.bottom() + 60);
    btnOptimize.set_pos(255, graph.bottom() + 90);
    btnStoreGesture.set_pos(255, graph.bottom() + 120);
    btnStoreCsv.set_pos(255, graph.bottom() + 150);
    btnClear.set_pos(255, graph.bottom() + 180);
    targetGrid.set_pos(395, graph.bottom() + 30);
    tabs.set_pos(23, graph.bottom() + 30);

    btnLoadTextGrid.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
    btnLoadPitchTier.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
    btnOptimize.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
    btnStoreCsv.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
    btnClear.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());

    // Tell the named rectangle to position itself such that it fits around the
    // tabbed display that contains the probability tables and the label at the top of the
    // screen.
    recOptions.wrap_around(tabs.get_rect());
    recTargets.wrap_around(targetGrid.get_rect());
    recActions.wrap_around(btnLoadTextGrid.get_rect() + btnClear.get_rect());
}

void MainWindow::update()
{
    graph.update();
}

void MainWindow::openTextGrid(const std::string& fileName)
{
    try
    {
        // process TextGrid input
        TextGridReader tgreader(fileName);
        Data::getInstance().syllableBoundaries = tgreader.getBounds();
        update();
    }
    catch (std::exception & e)
    {
        message_box("Error", "Unable to load TextGrid file " + fileName + "\n" + e.what());
    }
}

void MainWindow::openPitchTier(const std::string& fileName)
{
    try
    {
        // process PitchTier input
        PitchTierReader ptreader(fileName);
        Data::getInstance().originalF0 = ptreader.getF0();
        set_title("Target Optimizer - " + left_substr(right_substr(fileName, "\\/"), "."));
        update();
    }
    catch (std::exception & e)
    {
        message_box("Error", "Unable to load PitchTier file " + fileName + "\n" + e.what());
    }
}

void MainWindow::onButtonTextGridOpen()
{
    // display a file chooser window and when the user choses a file
    // call the on_open_file_selected() function
    open_existing_file_box(*this, &MainWindow::openTextGrid);
    if (!Data::getInstance().originalF0.empty())
    {
        onReadyForOptimize();
    }
}

void MainWindow::onButtonPitchTierOpen()
{
    // display a file chooser window and when the user choses a file
    // call the on_open_file_selected() function
    open_existing_file_box(*this, &MainWindow::openPitchTier);
    if (!(Data::getInstance().syllableBoundaries.empty()))
    {
        onReadyForOptimize();
    }
}

void MainWindow::onReadyForOptimize()
{
    btnOptimize.enable();
}

void MainWindow::onButtonOptimize()
{
    // main task
    ParameterSet parameters;
    try
    {
        parameters = readParameters();
    }
    catch (std::exception & e)
    {
        std::string msg(e.what());
        message_box("Error", "Wrong option input \n" + msg);
        return;
    }

    OptimizationProblem problem(parameters, Data::getInstance().originalF0, Data::getInstance().syllableBoundaries);
    BobyqaOptimizer optimizer;
    optimizer.optimize(problem);
    Data::getInstance().pitchTargets = problem.getPitchTargets();
    Data::getInstance().optimalF0 = problem.getModelF0();
    Data::getInstance().onset = problem.getOnset();
    //graph.setTargets(m_optTarget);
    //graph.setOptimalF0(m_optF0);

    // show target values in grid
    targetGrid.set_grid_size(5, Data::getInstance().pitchTargets.size() + 2);
    targetGrid.set_text(1, 0, "slope [st/s]");
    targetGrid.set_text(2, 0, "offset [st]");
    targetGrid.set_text(3, 0, "tau [ms]");
    targetGrid.set_text(4, 0, "duration [s]");
    targetGrid.set_background_color(1, 0, rgb_pixel(200, 250, 250));
    targetGrid.set_background_color(2, 0, rgb_pixel(200, 250, 250));
    targetGrid.set_background_color(3, 0, rgb_pixel(200, 250, 250));
    targetGrid.set_background_color(4, 0, rgb_pixel(200, 250, 250));
    targetGrid.set_text(0, 0, "parameter");
    targetGrid.set_background_color(0, 0, rgb_pixel(170, 220, 220));

    for (unsigned i = 0; i < Data::getInstance().pitchTargets.size(); ++i)
    {
        std::ostringstream value; value << std::fixed << std::setprecision(0); value << i + 1;
        targetGrid.set_text(0, i + 1, "target[" + value.str() + "]");
        targetGrid.set_background_color(0, i + 1, rgb_pixel(170, 220, 220));
        value.str(""); value.clear(); value << std::fixed << std::setprecision(3);
        value << Data::getInstance().pitchTargets[i].slope; targetGrid.set_text(1, i + 1, value.str()); value.str(""); value.clear();
        value << Data::getInstance().pitchTargets[i].offset; targetGrid.set_text(2, i + 1, value.str()); value.str(""); value.clear();
        value << Data::getInstance().pitchTargets[i].tau; targetGrid.set_text(3, i + 1, value.str()); value.str(""); value.clear();
        value << Data::getInstance().pitchTargets[i].duration; targetGrid.set_text(4, i + 1, value.str()); value.str(""); value.clear();
    }

    // set non-editable
    for (unsigned i = 0; i < targetGrid.number_of_rows(); ++i)
    {
        for (unsigned j = 0; j < targetGrid.number_of_columns(); ++j)
        {
            targetGrid.set_editable(i, j, false);
        }
    }
    unblockMainWindow();
    std::ostringstream msg;
    msg << "Optimization successful!\nRMSE = " << problem.getRootMeanSquareError() << " st\nCORR = " << problem.getCorrelationCoefficient();
    message_box("Information", msg.str());
    //message_box_blocking("Information", msg.str());
}

ParameterSet MainWindow::readParameters()
{
    //calculate mean f0
    double meanF0 = 0.0;
    for (unsigned i = 0; i < Data::getInstance().originalF0.size(); ++i)
    {
        meanF0 += Data::getInstance().originalF0[i].value;
    }
    meanF0 /= Data::getInstance().originalF0.size();

    ParameterSet parameters;

    parameters.deltaSlope = atof(tfSlope.text().c_str());
    parameters.deltaOffset = atof(tfOffset.text().c_str());
    parameters.deltaTau = atof(tfTau.text().c_str());
    parameters.weightSlope = atof(tfWeightm.text().c_str());
    parameters.weightOffset = atof(tfWeightb.text().c_str());
    parameters.weightTau = atof(tfWeighttau.text().c_str());
    parameters.lambda = atof(tfLambda.text().c_str());
    parameters.meanSlope = 0.0;
    parameters.meanOffset = meanF0;
    parameters.meanTau = 15.0;

    // check input
    if (parameters.deltaSlope == 0.0)
        throw dlib::error("\"" + tfSlope.text() + "\" is no valid value for slope search space");
    if (parameters.deltaOffset == 0.0)
        throw dlib::error("\"" + tfOffset.text() + "\" is no valid value for offset search space");
    if (parameters.deltaTau == 0.0)
        throw dlib::error("\"" + tfTau.text() + "\" is no valid value for tau search space");

    if (parameters.weightSlope == 0.0 && !(!tfWeightm.text().compare("0.0") || !tfWeightm.text().compare("0")))
        throw dlib::error("\"" + tfWeightm.text() + "\" is no valid value for slope weight");

    if (parameters.weightOffset == 0.0 && !(!tfWeightb.text().compare("0.0") || !tfWeightb.text().compare("0")))
        throw dlib::error("\"" + tfWeightb.text() + "\" is no valid value for offset weight");

    if (parameters.weightTau == 0.0 && !(!tfWeighttau.text().compare("0.0") || !tfWeighttau.text().compare("0")))
        throw dlib::error("\"" + tfWeighttau.text() + "\" is no valid value for tau weight");

    if (parameters.lambda == 0.0 && !(!tfLambda.text().compare("0.0") || !tfLambda.text().compare("0")))
        throw dlib::error("\"" + tfLambda.text() + "\" is no valid value for lambda");

    // every other wrong input gets 0.0 due to atof()

    return parameters;
}

void MainWindow::onButtonClear()
{
    // Reset data
    Data::getInstance().reset();

    // clear text
    for (unsigned i = 0; i < targetGrid.number_of_rows(); ++i)
    {
        for (unsigned j = 0; j < targetGrid.number_of_columns(); ++j)
        {
            targetGrid.set_text(i, j, "");
        }
    }

    noOptimizationPerformed();
    update();
}

// This event is called when the user choses which file to save the graph to
void MainWindow::onSaveFileGesture(const std::string& fileName)
{
    std::string name = fileName;
    if (name.find(".ges") == std::string::npos)
    {
        name += ".ges";
    }
    GestureWriter gwriter(name);
    gwriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
}

// This event is called when the user selects from the menu bar File->Save As
void MainWindow::onButtonSaveAsGesture()
{
    save_file_box(*this, &MainWindow::onSaveFileGesture);
}

// This event is called when the user choses which file to save the graph to
void MainWindow::onSaveFileCsv(const std::string& fileName)
{
    std::string name = fileName;
    if (name.find(".csv") == std::string::npos)
    {
        name += ".csv";
    }
    CsvWriter cwriter(name);
    cwriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
}

// This event is called when the user selects from the menu bar File->Save As
void MainWindow::onMenuSaveAsCsv()
{
    save_file_box(*this, &MainWindow::onSaveFileCsv);
}

// This event is called when the user choses which file to save the graph to
void MainWindow::onSaveFilePitchTier(const std::string& fileName)
{
    std::string name = fileName;
    if (name.find(".PitchTier") == std::string::npos)
    {
        name += ".PitchTier";
    }
    PitchTierWriter pwriter(name);
    pwriter.writeF0(Data::getInstance().optimalF0);
}

// This event is called when the user selects from the menu bar File->Save As
void MainWindow::onMenuSaveAsPitchTier()
{
    save_file_box(*this, &MainWindow::onSaveFilePitchTier);
}

void MainWindow::unblockMainWindow()
{
    tabs.enable();
    targetGrid.enable();
    btnLoadTextGrid.enable();
    btnLoadPitchTier.enable();
    btnOptimize.enable();
    btnClear.enable();
    btnStoreGesture.enable();
    btnStoreCsv.enable();
    graph.enable();
    mbar.enable();
}

void MainWindow::onMenuFileQuit()
{
    close_window();
}

void MainWindow::onMenuFileHelp()
{
    message_box("Help",
        "(1) Load Praat-TextGrid file (Praat -> Save as short text file...).\n"
        "(2) Load Praat-PitchTier file (Praat -> Save as PitchTier spreadsheet file...).\n"
        "(3) Choose options and start optimization.\n"
        "(4) Save targets as VTL gesture or csv file. Save modeled f0 as PitchTier file.\n");
}

void MainWindow::onMenuFileAbout()
{
    message_box("About", "This application calculates the optimal Pitch Targets according to the \n"
        "Target-Approximation-Model utilizing a 5th order low pass filter.\n\n"
        "Version 1.0\n\n"
        "by Patrick Schmager");
}

