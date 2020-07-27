#include <iostream>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/filename.h>
#include "wxMainWindow.h"
#include "SearchSpacePage.h"
#include "RegularizationPage.h"

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
static const int IDC_TARGET_DISPLAY = wxNewId();

wxBEGIN_EVENT_TABLE(wxMainWindow, wxFrame)
EVT_MENU(IDM_CLEAR, OnClear)
EVT_MENU(IDM_OPEN_TEXTGRID, OnOpenTextGrid)
EVT_MENU(IDM_OPEN_PITCHTIER, OnOpenPitchTier)

EVT_BUTTON(IDB_CLEAR, OnClear)
EVT_BUTTON(IDB_OPEN_TEXTGRID, OnOpenTextGrid)
EVT_BUTTON(IDB_OPEN_PITCHTIER, OnOpenPitchTier)
wxEND_EVENT_TABLE()

wxMainWindow::wxMainWindow(const wxString& title, const wxPoint& pos, const wxSize& size) 
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	this->SetBackgroundColour(*wxWHITE);
	
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

	// Options
	wxStaticBoxSizer* optionsSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Options"));
	// The notebook with optimization options
	wxNotebook* notebook{ new wxNotebook(this, wxID_ANY) };
	// The first page of the notebook with search space options
	SearchSpacePage* searchSpacePage{ new SearchSpacePage(notebook, wxID_ANY) };
	notebook->AddPage(searchSpacePage, wxT("Search Space"));
	// The second page of the notebook with regularization options
	RegularizationPage* regularizationPage{ new RegularizationPage(notebook, wxID_ANY) };
	notebook->AddPage(regularizationPage, wxT("Regularization"));
	optionsSizer->Add(notebook, wxSizerFlags().Align(wxCENTER).Border(wxALL, 5).Expand());

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
	wxTextCtrl* targetTextCtrl{ new wxTextCtrl(this, IDC_TARGET_DISPLAY, wxEmptyString, wxDefaultPosition, wxSize(400,300), wxTE_MULTILINE) };
	targetsSizer->Add(targetTextCtrl, wxSizerFlags().Expand().Border(wxALL, 5));

	bottomSizer->Add(targetsSizer, wxSizerFlags().Align(wxCENTER).Border(wxALL, 5).Expand());
	
	topLevelSizer->Add(bottomSizer, wxSizerFlags().Expand());
	
	updateWidgets();
	this->SetSizerAndFit(topLevelSizer);
}

void wxMainWindow::OnClear(wxCommandEvent& event)
{
	// Reset data
	Data::getInstance().reset();
	isAnyDataLoaded = false;
	isAllDataLoaded = false;
	isOptimized = false;

	updateWidgets();
}

void wxMainWindow::OnOpenTextGrid(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, wxT("Open TextGrid file"), "", "",
			"TextGrid files (*.TextGrid)|*.TextGrid", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     
	
	
	TextGridReader tgreader(openFileDialog.GetPath().ToStdString());
	Data::getInstance().syllableBoundaries = tgreader.getBounds();
	this->SetTitle(wxT("Target Optimizer - ") + wxFileName(openFileDialog.GetFilename()).GetName());

	if (isAnyDataLoaded)
	{
		isAllDataLoaded = true;
	}
	else
	{
		isAnyDataLoaded = true;
	}

	updateWidgets();
}

void wxMainWindow::OnOpenPitchTier(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, wxT("Open PitchTier file"), "", "",
		"PitchTier files (*.PitchTier)|*.PitchTier", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	PitchTierReader ptreader(openFileDialog.GetPath().ToStdString());
	Data::getInstance().originalF0 = ptreader.getF0();
	this->SetTitle(wxT("Target Optimizer - ") + wxFileName(openFileDialog.GetFilename()).GetName());
	
	if (isAnyDataLoaded) 
	{ 
		isAllDataLoaded = true; 
	}
	else
	{
		isAnyDataLoaded = true;
	}

	
	updateWidgets();
}


void wxMainWindow::updateWidgets()
{
	// Clear is only available when any of the files are loaded
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_CLEAR))->Enable(isAnyDataLoaded);
	this->GetMenuBar()->Enable(IDM_CLEAR, isAnyDataLoaded);

	// Optimization is only available if all necessary files are loaded
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_OPTIMIZE))->Enable(isAllDataLoaded);
	this->GetMenuBar()->Enable(IDM_OPTIMIZE, isAllDataLoaded);
	
	// Saving files is only available after optimization
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_GESTURES))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_GESTURES, isOptimized);
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_CSV))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_CSV, isOptimized);
	static_cast<wxButton*>(wxWindow::FindWindowById(IDB_SAVE_PITCHTIER))->Enable(isOptimized);
	this->GetMenuBar()->Enable(IDM_SAVE_PITCHTIER, isOptimized);
	
	// The pitch target display is only available after optimization
	static_cast<wxTextCtrl*>(wxWindow::FindWindowById(IDC_TARGET_DISPLAY))->Enable(isOptimized);

	plotRegion->Refresh();

}
