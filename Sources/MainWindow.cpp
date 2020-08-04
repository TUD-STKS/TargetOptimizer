#ifdef USE_WXWIDGETS
#include <iostream>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/filename.h>
#include "MainWindow.h"
#include "BobyqaOptimizer.h"


/* Menu IDs */
static const int IDM_OPEN_TEXTGRID = wxNewId();
static const int IDM_OPEN_PITCHTIER = wxNewId();
static const int IDM_OPTIMIZE = wxNewId();
static const int IDM_SAVE_GESTURES = wxNewId();
static const int IDM_SAVE_CSV = wxNewId();
static const int IDM_SAVE_PITCHTIER = wxNewId();
static const int IDM_CLEAR = wxNewId();
/* Buit-in IDs in use */
// wxID_EXIT
// wxID_HELP
// wxID_ABOUT

/* Button IDs */
static const int IDB_OPEN_TEXTGRID = wxNewId();
static const int IDB_OPEN_PITCHTIER = wxNewId();
static const int IDB_OPTIMIZE = wxNewId();
static const int IDB_SAVE_GESTURES = wxNewId();
static const int IDB_SAVE_CSV = wxNewId();
static const int IDB_SAVE_PITCHTIER = wxNewId();
static const int IDB_CLEAR = wxNewId();

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
EVT_MENU(IDM_OPEN_TEXTGRID, OnOpenTextGrid)
EVT_MENU(IDM_OPEN_PITCHTIER, OnOpenPitchTier)
EVT_MENU(IDM_OPTIMIZE, OnOptimize)
EVT_MENU(IDM_SAVE_GESTURES, OnSaveAsGesture)
EVT_MENU(IDM_SAVE_CSV, OnSaveAsCsv)
EVT_MENU(IDM_SAVE_PITCHTIER, OnSaveAsPitchTier)
EVT_MENU(wxID_HELP, OnHelp)
EVT_MENU(wxID_ABOUT, OnAbout)
EVT_MENU(wxID_EXIT, OnQuit)

EVT_BUTTON(IDB_CLEAR, OnClear)
EVT_BUTTON(IDB_OPEN_TEXTGRID, OnOpenTextGrid)
EVT_BUTTON(IDB_OPEN_PITCHTIER, OnOpenPitchTier)
EVT_BUTTON(IDB_OPTIMIZE, OnOptimize)
EVT_BUTTON(IDB_SAVE_GESTURES, OnSaveAsGesture)
EVT_BUTTON(IDB_SAVE_CSV, OnSaveAsCsv)
EVT_BUTTON(IDB_SAVE_PITCHTIER, OnSaveAsPitchTier)

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
	menu->Append(IDM_OPEN_TEXTGRID, wxT("Open &TextGrid"));
	menu->Append(IDM_OPEN_PITCHTIER, wxT("Open &PitchTier"));
	menu->Append(IDM_OPTIMIZE, wxT("&Optimize"));
	menu->Append(IDM_SAVE_GESTURES, wxT("Save as &GesturalScore"));
	menu->Append(IDM_SAVE_CSV, wxT("Save as &CSV"));
	menu->Append(IDM_SAVE_PITCHTIER, wxT("Save as P&itchTier"));
	menu->Append(IDM_CLEAR, wxT("C&lear"));
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
	wxButton* button{ new wxButton(this, IDB_OPEN_TEXTGRID, wxT("Open TextGrid")) };
	wxSizerFlags buttonFlags;
	buttonFlags.Expand().Border(wxALL, 5);
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_OPEN_PITCHTIER, wxT("Open PitchTier"));
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_OPTIMIZE, wxT("Optimize"));
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_SAVE_GESTURES, wxT("Save as Gestural Score"));
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_SAVE_CSV, wxT("Save as CSV"));
	actionsSizer->Add(button, buttonFlags);
	button = new wxButton(this, IDB_SAVE_PITCHTIER, wxT("Save as PitchTier"));
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
	isPitchTierLoaded = false;
	isOptimized = false;

	//resultsTable->ClearGrid();
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
	std::cout << "OnBoundaryCellChanged called!"<< std::endl;
	double Cell;
	Data::getInstance().syllableBoundaries.at( event.GetCol() ) = targetOptions->boundaryPage->boundaryTable->GetCellValue( event.GetRow(), event.GetCol() ).ToDouble(&Cell);

	//if(!number.ToDouble(&value)){ /* error! */ } // TODO: hiermit error abfangen falls jemand etwas anderes als eine Zahl eingibt!!

	
	//isOptimized = false;
	//isPitchTierLoaded = true;
	
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

void MainWindow::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void MainWindow::OnOpenTextGrid(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, wxT("Open TextGrid file"), "", "",
			"TextGrid files (*.TextGrid)|*.TextGrid", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     

	if (isOptimized)
	{
		this->clear();
	}
	
	TextGridReader tgreader(std::string(openFileDialog.GetPath().utf8_str()));
	Data::getInstance().syllableBoundaries = tgreader.getBounds();
	this->SetTitle(wxT("Target Optimizer - ") + wxFileName(openFileDialog.GetFilename()).GetName());


	isOptimized = false;
	isTextGridLoaded = true;
	updateWidgets();
}

void MainWindow::OnOpenPitchTier(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, wxT("Open PitchTier file"), "", "",
		"PitchTier files (*.PitchTier)|*.PitchTier", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	if (isOptimized)
	{
		this->clear();
	}
	PitchTierReader ptreader(std::string(openFileDialog.GetPath().utf8_str()));
	Data::getInstance().originalF0 = ptreader.getF0();
	this->SetTitle(wxT("Target Optimizer - ") + wxFileName(openFileDialog.GetFilename()).GetName());
	
	isOptimized = false;
	isPitchTierLoaded = true;
	
	updateWidgets();
}

void MainWindow::OnOptimize(wxCommandEvent& event)
{
	auto options = optimizationOptions->getOptions();

	if (options.deltaOffset == 0 || options.deltaSlope == 0 || options.deltaTau == 0)
	{
		wxMessageBox(wxT("Error: 0 is not a valid search space parameter!"), wxT("Parameter error"), wxICON_ERROR);
		return;
	}

	OptimizationProblem problem(options,
		Data::getInstance().originalF0,
		Data::getInstance().syllableBoundaries);
	BobyqaOptimizer optimizer;
	try
	{
		optimizer.optimize(problem);
	}
	catch (const std::exception&)
	{
		wxMessageBox(wxT("Something went wrong during the optimization. Did you choose matching TextGrid and PitchTier files?"), wxT("Error"), wxICON_ERROR);
		return;
	}	
	Data::getInstance().pitchTargets = problem.getPitchTargets();
	Data::getInstance().optimalF0 = problem.getModelF0();
	Data::getInstance().onset = problem.getOnset();
	//Data::getInstance().syllableBoundaries = problem.getBoundaries();


	std::ostringstream msg;
	msg << "Optimization successful!\nRMSE = " << problem.getRootMeanSquareError() << " st\nCORR = " << problem.getCorrelationCoefficient();
	wxMessageBox(msg.str(), wxT("Information"));
	isOptimized = true;
	updateWidgets();
}

void MainWindow::OnSaveAsGesture(wxCommandEvent& event)
{
	wxString defaultName = this->GetTitle().AfterFirst('-').Trim();
	wxFileDialog saveFileDialog(this, wxT("Save Gestural Score file"), "", defaultName,
			"Gestural Score files (*.ges)|*.ges", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	GestureWriter gwriter(std::string(saveFileDialog.GetPath().utf8_str()));
	gwriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
}

void MainWindow::OnSaveAsCsv(wxCommandEvent& event)
{
	wxString defaultName = this->GetTitle().AfterFirst('-').Trim();
	wxFileDialog saveFileDialog(this, wxT("Save CSV file"), "", defaultName,
		"CSV (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	CsvWriter cwriter(std::string(saveFileDialog.GetPath().utf8_str()));
	cwriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
}

void MainWindow::OnSaveAsPitchTier(wxCommandEvent& event)
{
	wxString defaultName = this->GetTitle().AfterFirst('-').Trim();
	wxFileDialog saveFileDialog(this, wxT("Save PitchTier file"), "", defaultName,
		"PitchTier (*.PitchTier)|*.PitchTier", wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	
	PitchTierWriter pwriter(std::string(saveFileDialog.GetPath().utf8_str()));
	pwriter.writeF0(Data::getInstance().optimalF0);
}


void MainWindow::updateWidgets()
{
	// Clear is only available when any of the files are loaded
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_CLEAR))->Enable(isTextGridLoaded || isPitchTierLoaded);
	this->GetMenuBar()->Enable(IDM_CLEAR, isTextGridLoaded || isPitchTierLoaded);

	// Optimization is only available if all necessary files are loaded
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_OPTIMIZE))->Enable(isTextGridLoaded && isPitchTierLoaded);
	this->GetMenuBar()->Enable(IDM_OPTIMIZE, isTextGridLoaded && isPitchTierLoaded);
	
	// Saving files is only available after optimization
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_GESTURES))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_GESTURES, isOptimized);
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_CSV))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_CSV, isOptimized);
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_PITCHTIER))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_PITCHTIER, isOptimized);
	
	// The pitch target display is only available after optimization
	//static_cast<wxGrid*>(wxWindow::FindWindowById(IDC_TARGET_DISPLAY))->Enable(isOptimized);
	targetOptions->boundaryPage->Enable(isTextGridLoaded);

	if (!Data::getInstance().syllableBoundaries.empty())
	{
		//std::cout << Data::getInstance().syllableBoundaries.at(0) << std::endl;
		targetOptions->boundaryPage->setEntries( Data::getInstance().syllableBoundaries );
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
