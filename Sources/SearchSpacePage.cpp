#ifdef USE_WXWIDGETS

#include "SearchSpacePage.h"

SearchSpacePage::SearchSpacePage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// Get default parameters
	SearchSpaceParameters defaults;

	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(3) };
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT|wxLEFT, 5);
	wxStaticText* plusMinus{ new wxStaticText(this, wxID_ANY, wxT("+/-")) };
	wxSizerFlags plusMinusFlags;
	plusMinusFlags.Align(wxRIGHT | wxALIGN_CENTER_VERTICAL).Border(wxLEFT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);
	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("Slope 0.0 [st/s]")) };
	optionsSizer->Add(label, labelFlags);
	optionsSizer->Add(plusMinus, plusMinusFlags);
	deltaSlopeCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.deltaSlope), wxDefaultPosition, wxDefaultSize,  wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.deltaSlope, 1);
	optionsSizer->Add(deltaSlopeCtrl, valueFlags);
	label = new wxStaticText(this, wxID_ANY, wxT("Offset mean f0 [st]"));
	optionsSizer->Add(label, labelFlags);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	deltaOffsetCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.deltaOffset), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.deltaOffset, 1);
	optionsSizer->Add(deltaOffsetCtrl, valueFlags);
	label = new wxStaticText(this, wxID_ANY, wxT("Tau [ms]"));
	optionsSizer->Add(label, labelFlags);
	optionsSizer->AddSpacer(0);
	meanTauCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.meanTau), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.meanTau, 0.5);
	optionsSizer->Add(meanTauCtrl, valueFlags);
	optionsSizer->AddSpacer(0);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	deltaTauCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.deltaTau), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.deltaTau, 1);
	optionsSizer->Add(deltaTauCtrl, valueFlags);
	label = new wxStaticText(this, wxID_ANY, wxT("Boundary delta [%]"));
	optionsSizer->Add(label, labelFlags);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	deltaBoundaryCtrl = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.deltaBoundary), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 500, defaults.deltaBoundary, 1);
	optionsSizer->Add(deltaBoundaryCtrl, valueFlags);

	label = new wxStaticText(this, wxID_ANY, wxT("Init bounds"));
	optionsSizer->Add(label, labelFlags);
	plusMinus = new wxStaticText(this, wxID_ANY, wxT("+/-"));
	optionsSizer->Add(plusMinus, plusMinusFlags);
	initBoundsCtrl = new wxSpinCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), defaults.initBounds), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.initBounds);//, 1);
	optionsSizer->Add(initBoundsCtrl, valueFlags);
	this->SetSizer(optionsSizer);
}

SearchSpaceParameters SearchSpacePage::getParameters()
{
	SearchSpaceParameters params;
	params.deltaSlope = deltaSlopeCtrl->GetValue();
	params.deltaOffset = deltaOffsetCtrl->GetValue();
	params.deltaTau = deltaTauCtrl->GetValue();
	params.meanTau = meanTauCtrl->GetValue();
	params.deltaBoundary = deltaBoundaryCtrl->GetValue();
	params.initBounds = initBoundsCtrl->GetValue();

	params.optimizeBoundaries = (params.deltaBoundary != 0);
	if (params.optimizeBoundaries)
	{
		params.numberOptVar = 4;
	}
	else
	{
		params.numberOptVar = 3;
	}
	std::cout << "Number of opt var: " << params.numberOptVar << std::endl;


	return params;
}

void SearchSpacePage::setParameters(SearchSpaceParameters newParams)
{
	deltaSlopeCtrl->SetValue(newParams.deltaSlope);
	deltaOffsetCtrl->SetValue(newParams.deltaOffset);
	deltaTauCtrl->SetValue(newParams.deltaTau);
	meanTauCtrl->SetValue(newParams.meanTau);
	deltaBoundaryCtrl->SetValue(newParams.deltaBoundary);
	initBoundsCtrl->SetValue(newParams.initBounds);
}

#endif
