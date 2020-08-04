#ifdef USE_WXWIDGETS

#include "SearchSpacePage.h"

SearchSpacePage::SearchSpacePage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(3) };
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT|wxLEFT, 5);
	wxStaticText* plusMinus{ new wxStaticText(this, wxID_ANY, wxT("+/-")) };
	wxSizerFlags plusMinusFlags;
	plusMinusFlags.Align(wxRIGHT | wxALIGN_CENTER_VERTICAL).Border(wxLEFT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);
	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("slope 0.0 [st/s]")) };
	optionsSizer->Add(label, labelFlags);
	optionsSizer->Add(plusMinus, plusMinusFlags);
	slopeDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("50.0"), wxDefaultPosition, wxDefaultSize,  wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 50, 1);
	optionsSizer->Add(slopeDeltaCtrl, valueFlags);
	label = new wxStaticText(this, wxID_ANY, wxT("offset f0-mean [st]"));
	optionsSizer->Add(label, labelFlags);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	offsetDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("20.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 20, 1);
	optionsSizer->Add(offsetDeltaCtrl, valueFlags);
	label = new wxStaticText(this, wxID_ANY, wxT("tau 15.0 [ms]"));
	optionsSizer->Add(label, labelFlags);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	tauDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("5.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 5, 1);
	optionsSizer->Add(tauDeltaCtrl, valueFlags);
	label = new wxStaticText(this, wxID_ANY, wxT("boundary delta [ms]"));
	optionsSizer->Add(label, labelFlags);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	boundaryDeltaCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxT("40.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 40, 1);
	optionsSizer->Add(boundaryDeltaCtrl, valueFlags);
	this->SetSizer(optionsSizer);
}

SearchSpaceParameters SearchSpacePage::getParameters()
{
	SearchSpaceParameters params;
	params.slopeDelta = slopeDeltaCtrl->GetValue();
	params.offsetDelta = offsetDeltaCtrl->GetValue();
	params.tauDelta = tauDeltaCtrl->GetValue();
	params.boundaryDelta = boundaryDeltaCtrl->GetValue();
	return params;
}

#endif
