#ifdef USE_WXWIDGETS

#include "ResultPage.h"
/* Control IDs */
static const int IDC_TARGET_DISPLAY = wxNewId();

ResultPage::ResultPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	wxFlexGridSizer* resultsSizer{ new wxFlexGridSizer(3) };
	resultsTable = new wxGrid(this, IDC_TARGET_DISPLAY);
	resultsTable->SetLabelBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	resultsTable->EnableEditing(false);
	resultsTable->CreateGrid(4, 1);
	resultsTable->SetRowLabelValue(0, wxT("Slope [st/s]"));
	resultsTable->SetRowLabelValue(1, wxT("Offset [st]"));
	resultsTable->SetRowLabelValue(2, wxT("Tau [ms]"));
	resultsTable->SetRowLabelValue(3, wxT("Duration [s]"));
	resultsTable->SetColLabelValue(0, wxEmptyString);
	resultsTable->SetRowLabelSize(wxGRID_AUTOSIZE);
	resultsTable->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
	resultsTable->SetColLabelSize(wxGRID_AUTOSIZE);
	resultsTable->SetColLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	resultsTable->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	resultsSizer->Add(resultsTable, wxSizerFlags(1).Expand().Border(wxALL, 5));
	this->SetSizer(resultsSizer);
	// A sizer to organize the option labels and values
	//wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(3) };
	//wxSizerFlags labelFlags;
	//labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT|wxLEFT, 5);
	//wxStaticText* plusMinus{ new wxStaticText(this, wxID_ANY, wxT("+/-")) };
	//wxSizerFlags plusMinusFlags;
	//plusMinusFlags.Align(wxRIGHT | wxALIGN_CENTER_VERTICAL).Border(wxLEFT, 5);
	//wxSizerFlags valueFlags;
	//valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);
	//wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("slope 0.0 [st/s]")) };
	//optionsSizer->Add(label, labelFlags);
	//optionsSizer->Add(plusMinus, plusMinusFlags);
	//slopeDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("50.0"), wxDefaultPosition, wxDefaultSize,  wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 50, 1);
	//optionsSizer->Add(slopeDeltaCtrl, valueFlags);
	//label = new wxStaticText(this, wxID_ANY, wxT("offset f0-mean [st]"));
	//optionsSizer->Add(label, labelFlags);
	//plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	//optionsSizer->Add(plusMinus, plusMinusFlags);
	//offsetDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("20.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 20, 1);
	//optionsSizer->Add(offsetDeltaCtrl, valueFlags);
	//label = new wxStaticText(this, wxID_ANY, wxT("tau 15.0 [ms]"));
	//optionsSizer->Add(label, labelFlags);
	//plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	//optionsSizer->Add(plusMinus, plusMinusFlags);
	//tauDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("5.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 5, 1);
	//optionsSizer->Add(tauDeltaCtrl, valueFlags);
	//label = new wxStaticText(this, wxID_ANY, wxT("boundary delta [ms]"));
	//optionsSizer->Add(label, labelFlags);
	//plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	//optionsSizer->Add(plusMinus, plusMinusFlags);
	//boundaryDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 0, 1);
	//optionsSizer->Add(boundaryDeltaCtrl, valueFlags);
	//this->SetSizer(resultsSizer);
}

ResultParameters ResultPage::getParameters()
{
	ResultParameters params;
	//params.slopeDelta = slopeDeltaCtrl->GetValue();
	//params.offsetDelta = offsetDeltaCtrl->GetValue();
	//params.tauDelta = tauDeltaCtrl->GetValue();
	//params.boundaryDelta = boundaryDeltaCtrl->GetValue();
	return params;
}

#endif
