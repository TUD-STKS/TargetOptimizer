#ifdef USE_WXWIDGETS

#include <wx/spinctrl.h>
#include "RegularizationPage.h"


RegularizationPage::RegularizationPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// Get default parameters
	RegularizationParameters defaults;

	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(2) };
	optionsSizer->AddGrowableCol(0);
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT | wxLEFT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);

	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("Lambda")) };
	optionsSizer->Add(label, labelFlags);
	lambda = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.lambda), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.lambda, 0.01);
	optionsSizer->Add(lambda, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("Slope weight"));
	optionsSizer->Add(label, labelFlags);	
	weightSlope = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.weightSlope), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.weightSlope, 0.01);
	optionsSizer->Add(weightSlope, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("Offset weight"));
	optionsSizer->Add(label, labelFlags);
	weightOffset = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.weightOffset), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.weightOffset, 0.01);
	optionsSizer->Add(weightOffset, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("Tau weight"));
	optionsSizer->Add(label, labelFlags);
	weightTau = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), defaults.weightTau), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 100, defaults.weightTau, 0.01);
	optionsSizer->Add(weightTau, valueFlags);
	
	this->SetSizer(optionsSizer);
}

RegularizationParameters RegularizationPage::getParameters()
{
	RegularizationParameters params;
	params.lambda = lambda->GetValue();
	params.weightSlope = weightSlope->GetValue();
	params.weightOffset = weightOffset->GetValue();
	params.weightTau = weightTau->GetValue();

	return params;
}

void RegularizationPage::setParameters(RegularizationParameters newParams)
{
	lambda->SetValue(newParams.lambda);
	weightSlope->SetValue(newParams.weightSlope);
	weightOffset->SetValue(newParams.weightOffset);
	weightTau->SetValue(newParams.weightTau);
}

#endif
