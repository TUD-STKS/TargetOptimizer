#ifdef USE_WXWIDGETS

#include <wx/spinctrl.h>
#include "RegularizationPage.h"


RegularizationPage::RegularizationPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(2) };
	optionsSizer->AddGrowableCol(0);
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT | wxLEFT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);

	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("lambda")) };
	optionsSizer->Add(label, labelFlags);
	lambda = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.01"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 0.01, 0.01);
	optionsSizer->Add(lambda, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("weight-slope"));
	optionsSizer->Add(label, labelFlags);	
	weightSlope = new wxSpinCtrlDouble(this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 1.0, 0.01);
	optionsSizer->Add(weightSlope, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("weight-offset"));
	optionsSizer->Add(label, labelFlags);
	weightOffset = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 0.5, 0.01);
	optionsSizer->Add(weightOffset, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("weight-tau"));
	optionsSizer->Add(label, labelFlags);
	weightTau = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, 0.1, 0.01);
	optionsSizer->Add(weightTau, valueFlags);
	
	this->SetSizer(optionsSizer);
}

RegularizationParameters RegularizationPage::getParameters()
{
	RegularizationParameters params;
	params.lambda = lambda->GetValue();
	params.weightSlope = weightSlope->GetValue();
	params.weightTau = weightTau->GetValue();

	return params;
}

#endif
