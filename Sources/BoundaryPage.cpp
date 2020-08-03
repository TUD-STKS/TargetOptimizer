#ifdef USE_WXWIDGETS

#include "BoundaryPage.h"
/* Control IDs */
static const int IDC_TARGET_DISPLAY = wxNewId();

BoundaryPage::BoundaryPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	wxFlexGridSizer* boundarySizer{ new wxFlexGridSizer(3) };
	boundaryTable = new wxGrid(this, IDC_TARGET_DISPLAY);
	boundaryTable->SetLabelBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	boundaryTable->EnableEditing(false);
	boundaryTable->CreateGrid(4, 1);
	boundaryTable->SetRowLabelValue(0, wxT("Slope [st/s]"));
	boundaryTable->SetRowLabelValue(1, wxT("Offset [st]"));
	boundaryTable->SetRowLabelValue(2, wxT("Tau [ms]"));
	boundaryTable->SetRowLabelValue(3, wxT("Duration [s]"));
	boundaryTable->SetColLabelValue(0, wxEmptyString);
	boundaryTable->SetRowLabelSize(wxGRID_AUTOSIZE);
	boundaryTable->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
	boundaryTable->SetColLabelSize(wxGRID_AUTOSIZE);
	boundaryTable->SetColLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	boundaryTable->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	boundarySizer->Add(boundaryTable, wxSizerFlags(1).Expand().Border(wxALL, 5));
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
	this->SetSizer(boundarySizer);
}

BoundaryParameters BoundaryPage::getParameters()
{
	BoundaryParameters params;
	//params.slopeDelta = slopeDeltaCtrl->GetValue();
	//params.offsetDelta = offsetDeltaCtrl->GetValue();
	//params.tauDelta = tauDeltaCtrl->GetValue();
	//params.boundaryDelta = boundaryDeltaCtrl->GetValue();
	return params;
}

#endif
