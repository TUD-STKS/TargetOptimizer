#ifdef USE_WXWIDGETS
#include <iostream>
#include <wx/choicdlg.h>
#include <wx/filename.h>
#include <wx/notebook.h>
#include <wx/progdlg.h>
#include <wx/spinctrl.h>
#include "MainWindow.h"
#include "BobyqaOptimizer.h"


/* Menu IDs */
static const int IDM_OPEN = wxNewId();
static const int IDM_OPTIMIZE = wxNewId();
static const int IDM_SAVE_AS = wxNewId();
static const int IDM_CLEAR = wxNewId();

static const int IDM_INIT_BOUNDS = wxNewId();

/* Buit-in IDs in use */
// wxID_EXIT
// wxID_HELP
// wxID_ABOUT

/* Button IDs */
static const int IDB_OPEN = wxNewId();
static const int IDB_OPTIMIZE = wxNewId();
static const int IDB_SAVE_AS = wxNewId();
static const int IDB_CLEAR = wxNewId();
static const int IDB_INIT_BOUNDS = wxNewId();

/* Control IDs */
//static const int IDC_TARGET_DISPLAY = wxNewId();
//static const int IDC_BOUNDARY_TABLE = wxNewId();
//static const int IDC_RESULT_TABLE = wxNewId();

/* */
static const int IDP_SEARCH_OPTIONS = wxNewId();
static const int IDP_REGULARIZATION_OPTIONS = wxNewId();


//typedef void (wxEvtHandler::*MyEventFunction)(MyEvent&);
//#define MyEventHandler(func) wxEVENT_HANDLER_CAST(MyEventFunction, func)
//wxDEFINE_EVENT(EVT_GRID_CELL_CHANGED, MyEvent);
//#define EVT_GRID(id, func) wx__DECLARE_EVT1(EVT_GRID_CELL_CHANGED, id, MyEventHandler(func))

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(IDM_CLEAR, OnClear)
EVT_MENU(IDM_OPEN, OnOpen)
EVT_MENU(IDM_OPTIMIZE, OnOptimize)
EVT_MENU(IDM_INIT_BOUNDS, OnInitBounds)
EVT_MENU(IDM_SAVE_AS, OnSaveAs)

EVT_MENU(wxID_HELP, OnHelp)
EVT_MENU(wxID_ABOUT, OnAbout)
EVT_MENU(wxID_EXIT, OnQuit)

EVT_BUTTON(IDB_CLEAR, OnClear)
EVT_BUTTON(IDB_OPEN, OnOpen)
EVT_BUTTON(IDB_OPTIMIZE, OnOptimize)
EVT_BUTTON(IDB_INIT_BOUNDS, OnInitBounds)
EVT_BUTTON(IDB_SAVE_AS, OnSaveAs)

EVT_GRID_CMD_CELL_CHANGED(IDC_BOUNDARY_TABLE, OnBoundaryCellChanged)

wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	// The top level sizer for the main window
	wxBoxSizer* topLevelSizer(new wxBoxSizer(wxVERTICAL));

	// The sizer for the bottom part of the window containing
	wxBoxSizer* bottomSizer(new wxBoxSizer(wxHORIZONTAL));

	// Create the menu bar
	wxMenuBar* menuBar(new wxMenuBar());
	// File menu
	wxMenu* menu(new wxMenu());

	menu->Append(IDM_OPEN, wxT("&Open File(s)..."));
	menu->Append(IDM_OPTIMIZE, wxT("Optimize &Targets"));
	menu->Append(IDM_SAVE_AS, wxT("&Save as..."));
	menu->Append(IDM_INIT_BOUNDS, wxT("&Init Bounds"));
	menu->Append(IDM_CLEAR, wxT("&Clear"));
	menu->AppendSeparator();
	menu->Append(wxID_EXIT, wxT("&Quit"));
	menuBar->Append(menu, wxT("&File"));
	// Help menu
	menu = new wxMenu();
	menu->Append(wxID_HELP, wxT("&Help"));
	menu->Append(wxID_ABOUT, wxT("&About"));
	menuBar->Append(menu, wxT("&Help"));

	this->SetMenuBar(menuBar);


	plotRegion->SetMinSize(wxSize(900, 600));
	topLevelSizer->Add(plotRegion, wxSizerFlags().Expand().Border(wxALL, 10).Proportion(1));


	/* Create controls in the bottom area */

	//TODO: Create class OptimizationOptions derived from wxNotebook
	// Options
	wxStaticBoxSizer* optionsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Options"));
	// The notebook with optimization options
	optimizationOptions = new OptionsNotebook(this, wxID_ANY);
	optionsSizer->Add(optimizationOptions, wxSizerFlags(1).Align(wxCENTER).Border(wxALL, 5).Expand());

	bottomSizer->Add(optionsSizer, wxSizerFlags().Border(wxALL, 5).Expand());

	// Actions
	wxStaticBoxSizer* actionsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Actions"));
	wxButton* button{ new wxButton(this, IDB_OPEN, wxT("Open File(s)")) };
	wxSizerFlags buttonFlags;
	buttonFlags.Expand().Border(wxALL, 5);
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_INIT_BOUNDS, wxT("Init Bounds"));
	actionsSizer->Add(button, buttonFlags);

	//button = new wxButton(this, IDB_SAVE_CSV, wxT("Save as CSV"));
	//actionsSizer->Add(button, buttonFlags);
	//button = new wxButton(this, IDB_SAVE_PITCHTIER, wxT("Save as PitchTier"));
	//actionsSizer->Add(button, buttonFlags);

	button = new wxButton(this, IDB_OPTIMIZE, wxT("Optimize Targets"));
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_SAVE_AS, wxT("Save as..."));
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_CLEAR, wxT("Clear"));
	actionsSizer->Add(button, buttonFlags);

	bottomSizer->Add(actionsSizer, wxSizerFlags().Border(wxALL, 5).Expand());

	

	// Targets
	wxStaticBoxSizer* targetsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Targets"));

	//// The notebook with optimization options
	//targetOptions = new OptionsNotebook(this, wxID_ANY);
	//targetsSizer->Add(targetOptions, wxSizerFlags(1).Align(wxCENTER).Border(wxALL, 5).Expand());
	//resultsTable = new wxGrid(this, IDC_TARGET_DISPLAY);
	//resultsTable->SetLabelBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	//resultsTable->EnableEditing(false);
	//resultsTable->CreateGrid(4, 1);
	//resultsTable->SetRowLabelValue(0, wxT("Slope [st/s]"));
	//resultsTable->SetRowLabelValue(1, wxT("Offset [st]"));
	//resultsTable->SetRowLabelValue(2, wxT("Tau [ms]"));
	//resultsTable->SetRowLabelValue(3, wxT("Duration [s]"));
	//resultsTable->SetColLabelValue(0, wxEmptyString);
	//resultsTable->SetRowLabelSize(wxGRID_AUTOSIZE);
	//resultsTable->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
	//resultsTable->SetColLabelSize(wxGRID_AUTOSIZE);
	//resultsTable->SetColLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	//resultsTable->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	//targetsSizer->Add(resultsTable, wxSizerFlags(1).Expand().Border(wxALL, 5));


	//wxStaticBoxSizer* optionsSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Targets"));
	//// The notebook with optimization options
	targetOptions = new TargetsNotebook(this, wxID_ANY);
	targetsSizer->Add(targetOptions, wxSizerFlags(1).Align(wxCENTER).Border(wxALL, 5).Expand());
	//bottomSizer->Add(optionsSizer2, wxSizerFlags().Border(wxALL, 5).Expand());



	bottomSizer->Add(targetsSizer, wxSizerFlags(1).Align(wxCENTER).Border(wxALL, 5).Expand());

	topLevelSizer->Add(bottomSizer, wxSizerFlags().Expand());

	updateWidgets();
	this->SetSizerAndFit(topLevelSizer);
}


void MainWindow::clear()
{
	// Reset data
	Data::getInstance().reset();
	isTextGridLoaded = false;
	isBoundariesInit = false;
	isPitchTierLoaded = false;
	isOptimized = false;

	targetOptions->ResetTargetTables();

	this->SetTitle("Target Optimizer");
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Target Optimizer"));
	info.SetVersion(wxT("2.0"));
	info.SetDescription(wxT("TargetOptimizer is a free and open-source PC software to estimate pitch targets according to the Target Approximation Model by Yi Xu. "));
	info.SetCopyright(wxT("(C) 2020 TU Dresden, IAS"));
	info.SetWebSite(wxT("https://github.com/TUD-STKS/TargetOptimizer"));
	info.AddDeveloper(wxT("Paul Krug"));
	info.AddDeveloper(wxT("Patrick Schmager"));
	info.AddDeveloper(wxT("Simon Stone"));
	info.AddDeveloper(wxT("Alexander Wilbrandt"));
	wxAboutBox(info);
}

void MainWindow::OnBoundaryCellChanged(wxGridEvent& event)
{
	double Cell;
	auto Cell_Str = targetOptions->boundaryPage->boundaryTable->GetCellValue( event.GetRow(), event.GetCol() );
	Cell_Str.ToDouble(&Cell);

	if ( !Cell_Str.ToDouble(&Cell) || (Cell < 0) ) 
	{
		wxMessageBox(wxT("Error: The cell entries must be positive numbers!"), wxT("Parameter error"), wxICON_ERROR);
		if (isOptimized)
		{
			targetOptions->boundaryPage->setEntries(Data::getInstance().optimalBoundaries);
		}
		else
		{
			targetOptions->boundaryPage->setEntries(Data::getInstance().initialBoundaries);
		}
		
		return;
	}

	if (isOptimized)
	{
		Data::getInstance().optimalBoundaries.at(event.GetCol()) = Cell;
	}
	else
	{
		Data::getInstance().initialBoundaries.at(event.GetCol()) = Cell;
	}
	
	updateWidgets();
}

void MainWindow::OnClear(wxCommandEvent& event)
{
	this->clear();
	updateWidgets();
}

void MainWindow::OnHelp(wxCommandEvent& event)
{
	wxMessageBox(wxT(
		"(1) Load Praat-TextGrid file (Praat -> Save as short text file...).\n"
		"(2) Load Praat-PitchTier file (Praat -> Save as PitchTier spreadsheet file...).\n"
		"(3) Choose options and start optimization.\n"
		"(4) Save targets as VTL gesture or CSV file. Save modeled f0 as PitchTier file.\n"), wxT("Help"));
}


void MainWindow::OnInitBounds(wxCommandEvent& event)
{
	std::cout << "begin fo time: " << Data::getInstance().originalF0.at(0).time << 
	" end time: " << Data::getInstance().originalF0.back().time << std::endl;

	auto problemParams = optimizationOptions->getOptions().problemParams;
	std::cout << "number of init bounds: " << problemParams.searchSpaceParameters.initBounds << std::endl;

	if (problemParams.searchSpaceParameters.initBounds > 1)
	{
		double pitch_start = Data::getInstance().originalF0.at(0).time;
		double pitch_end = Data::getInstance().originalF0.back().time;
		double pitch_interval = pitch_end - pitch_start;
		double step = pitch_interval / (problemParams.searchSpaceParameters.initBounds - 1);


		std::vector<double> initBoundaries;
		for (int i = 0; i < problemParams.searchSpaceParameters.initBounds; ++i)
		{
			initBoundaries.push_back(pitch_start + i * step);
		}
		Data::getInstance().pitchTargets.clear();
		Data::getInstance().optimalF0.clear();
		Data::getInstance().initialBoundaries = initBoundaries;
		Data::getInstance().optimalBoundaries.clear();
		initBoundaries.clear();
		isBoundariesInit = true;
	}
	else if( (problemParams.searchSpaceParameters.initBounds <= 1) && (!isTextGridLoaded) )
	{
		wxMessageBox(wxT("Error: There must be at least two boundaries!"), wxT("Parameter error"), wxICON_ERROR);
	}
	else if(isTextGridLoaded) // Initialize with TextGrid boundaries again
	{
		Data::getInstance().pitchTargets.clear();
		Data::getInstance().optimalF0.clear();
		Data::getInstance().initialBoundaries = tg.getBounds();		
		Data::getInstance().optimalBoundaries.clear();
	}
	
	isOptimized = false;
	updateWidgets();

}


void MainWindow::OnOpen(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, wxT("Open TextGrid and/or PitchTier file"), "", "",
		"TextGrid files and PitchTier files (*.TextGrid; *.PitchTier)|*.TextGrid;*.PitchTier", wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	/* Check file selection */
	wxArrayString selectedFiles;
	openFileDialog.GetPaths(selectedFiles);
	int numTextGrid = 0;
	int numPitchTier = 0;
	for (const auto& filename : selectedFiles)
	{
		numTextGrid += static_cast<int>(filename.EndsWith(wxT("TextGrid")));
		numPitchTier += static_cast<int>(filename.EndsWith(wxT("PitchTier")));
	}
	if (numTextGrid > 1 || numPitchTier > 1)
	{
		wxMessageBox(wxT("Please select at most one TextGrid and one PitchTier file."),
			wxT("Too many files"), wxICON_ERROR);
		return;
	}

	/* File selection is legal, so clear plot and load files */
	if (isOptimized)
	{
		this->clear();
	}

	for (const auto& filepath : selectedFiles)
	{
		if (filepath.EndsWith("TextGrid"))
		{
			tg = DataIO::readTextGridFile(std::string(filepath.utf8_str()));
			wxArrayString choices;
			for (const auto& tierName : tg.getIntervalTierNames())
			{
				choices.push_back(wxString(tierName));
			}
			wxSingleChoiceDialog intervalChoiceDialog(this,
				wxT("Please pick the interval tier that holds the initial target boundaries"),
				wxT("Pick boundary tier"),
				choices);
			try
			{
				if (intervalChoiceDialog.ShowModal() == wxID_OK)
				{
					tg.syllableBoundaryTierName = std::string(intervalChoiceDialog.GetStringSelection().utf8_str());
				}
				else
				{

				}
				Data::getInstance().initialBoundaries = tg.getBounds();
			}
			catch (std::runtime_error &e)
			{
				wxMessageBox(wxString(e.what()), wxT("Error"), wxICON_ERROR);
			}

			this->SetTitle(wxT("Target Optimizer - ") + wxFileName(filepath).GetName());

			isOptimized = false;
			isTextGridLoaded = true;
		}
		if (filepath.EndsWith("PitchTier"))
		{
			auto ptreader = DataIO::readPitchTierFile(std::string(filepath.utf8_str()));
			Data::getInstance().originalF0 = ptreader.getF0();
			this->SetTitle(wxT("Target Optimizer - ") + wxFileName(filepath).GetName());

			isOptimized = false;
			isPitchTierLoaded = true;
		}
	}

	// Set the patience value to the default according to the number of targets
	auto opts = optimizationOptions->getOptions();
	opts.optimizerOptions.patience = 15 * (Data::getInstance().initialBoundaries.size() - 1);
	optimizationOptions->setOptions(opts);


	updateWidgets();
}

void MainWindow::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void MainWindow::OnOptimize(wxCommandEvent& event)
{
	//for (int i = 0; i<2;++i)
	//{
	auto options = optimizationOptions->getOptions();
	auto parameters = options.problemParams;
	auto optimizerOptions = options.optimizerOptions;

	if (parameters.searchSpaceParameters.deltaOffset == 0 || parameters.searchSpaceParameters.deltaSlope == 0 || parameters.searchSpaceParameters.deltaTau == 0)
	{
		wxMessageBox(wxT("Error: 0 is not a valid search space parameter!"), wxT("Parameter error"), wxICON_ERROR);
		return;
	}
	if ( Data::getInstance().initialBoundaries.size() <= 1 )
	{
		wxMessageBox(wxT("Error: There must be at least two boundaries!"), wxT("Parameter error"), wxICON_ERROR);
		return;
	}

	OptimizationProblem problem(parameters,
		Data::getInstance().originalF0,
		Data::getInstance().initialBoundaries);
	
	BobyqaOptimizer optimizer;
	wxProgressDialog pd(wxT("Please wait"), wxT("Optimizing targets..."), options.optimizerOptions.maxIterations, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_SMOOTH);
	
	try
	{
		optimizer.optimize( problem, optimizerOptions, &pd);
	}
	catch (const std::exception&)
	{
		pd.Close();
		wxMessageBox(wxT("Something went wrong during the optimization. Did you choose matching TextGrid and PitchTier files?"), wxT("Error"), wxICON_ERROR);
		return;
	}
	
	pd.Close();

	Data::getInstance().pitchTargets = problem.getPitchTargets();
	Data::getInstance().optimalF0 = problem.getModelF0();
	Data::getInstance().onset = problem.getOnset();
	Data::getInstance().optimalBoundaries = problem.getBoundaries();


	std::ostringstream msg;
	msg << "Optimization successful!\nRMSE = " << problem.getRootMeanSquareError() << " st\nCORR = " << problem.getCorrelationCoefficient();
	wxMessageBox(msg.str(), wxT("Information"));
	isOptimized = true;
	updateWidgets();
	//}
}

void MainWindow::OnSaveAs(wxCommandEvent& event)
{
	wxString defaultName = this->GetTitle().AfterFirst('-').Trim();
	wxFileDialog saveFileDialog(this, wxT("Save as..."), "", defaultName,
		wxT("Gestural Score file (*.ges)|*.ges|CSV file (*.csv)|*.csv|PitchTier file (*.PitchTier)|*.PitchTier|All supported files (*.ges, *.csv, *.PitchTier, *.TO)|*.ges; *.csv; *.PitchTier; *.TO"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	if (saveFileDialog.GetPath().EndsWith(wxT("ges")))
	{
		DataIO::saveGesturalScore(Data::getInstance().onset, Data::getInstance().pitchTargets, std::string(saveFileDialog.GetPath().utf8_str()));
	}
	else if (saveFileDialog.GetPath().EndsWith(wxT("csv")))
	{
		DataIO::saveCsvFile(Data::getInstance().onset, Data::getInstance().pitchTargets, std::string(saveFileDialog.GetPath().utf8_str()));
	}
	else if (saveFileDialog.GetPath().EndsWith(wxT("PitchTier")))
	{
		DataIO::savePitchTier(Data::getInstance().optimalF0, std::string(saveFileDialog.GetPath().utf8_str()));
	}
	//else if (saveFileDialog.GetPath().EndsWith(wxT("TO")))
	//{
	//	DataIO::saveTOFile(std::string(saveFileDialog.GetPath().utf8_str()));
	//}
	else
	{
		wxMessageBox(wxT("Please use one of the supported file types!"), wxT("Unsupported file format"), wxICON_ERROR);
	}
}

void MainWindow::updateWidgets()
{
	// Clear is only available when any of the files are loaded or boundaries are initialized
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_CLEAR))->Enable(isTextGridLoaded || isPitchTierLoaded || isBoundariesInit);
	this->GetMenuBar()->Enable(IDM_CLEAR, isTextGridLoaded || isPitchTierLoaded || isBoundariesInit);

	// Optimization is only available if all necessary files are loaded
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_OPTIMIZE))->Enable((isBoundariesInit || isTextGridLoaded) && isPitchTierLoaded);
	this->GetMenuBar()->Enable(IDM_OPTIMIZE, (isBoundariesInit || isTextGridLoaded) && isPitchTierLoaded);

	// Init bounds available after pitchtier is loaded
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_INIT_BOUNDS))->Enable( isPitchTierLoaded );
	this->GetMenuBar()->Enable(IDM_INIT_BOUNDS, isPitchTierLoaded);
	
	// Saving files is only available after optimization
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_AS))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_AS, isOptimized);

	// The pitch target display is only available after optimization
	//static_cast<wxGrid*>(wxWindow::FindWindowById(IDC_TARGET_DISPLAY))->Enable(isOptimized);
	targetOptions->boundaryPage->Enable(isTextGridLoaded);

	if (!Data::getInstance().optimalBoundaries.empty())
	{
		//std::cout << Data::getInstance().syllableBoundaries.at(0) << std::endl;
		targetOptions->boundaryPage->setEntries( Data::getInstance().optimalBoundaries );
	}
	else if (!Data::getInstance().initialBoundaries.empty())
	{
		targetOptions->boundaryPage->setEntries(Data::getInstance().initialBoundaries);
	}

	targetOptions->resultPage->Enable(isOptimized);

	plotRegion->Refresh();

	// Add optimized targets to table
	if (!Data::getInstance().pitchTargets.empty())
	{
		targetOptions->resultPage->setEntries( Data::getInstance().pitchTargets );
//		// Resize result table to correct number of columns
//		int colDifference = Data::getInstance().pitchTargets.size() - resultsTable->GetNumberCols();
//		if (colDifference > 0) { resultsTable->InsertCols(0, colDifference); }
//		if (colDifference < 0) { resultsTable->DeleteCols(0, -1*colDifference); }
//		int col = 0;
//		for (const auto& target : Data::getInstance().pitchTargets)
//		{
//			resultsTable->SetColLabelValue(col, wxT("Target ") + wxString::Format(wxT("%i"), col));
//			resultsTable->SetCellValue(wxGridCellCoords(0, col), wxString::Format(wxT("%.3f"), target.slope));
//			resultsTable->SetCellValue(wxGridCellCoords(1, col), wxString::Format(wxT("%.3f"), target.offset));
//			resultsTable->SetCellValue(wxGridCellCoords(2, col), wxString::Format(wxT("%.3f"), target.tau));
//			resultsTable->SetCellValue(wxGridCellCoords(3, col), wxString::Format(wxT("%.3f"), target.duration));
//			col++;
//		}
	}

}

#endif // USE_WXWIDGETS
