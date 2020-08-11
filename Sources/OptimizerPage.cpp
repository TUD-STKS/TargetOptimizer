#ifdef USE_WXWIDGETS

#include "OptimizerPage.h"


OptimizerPage::OptimizerPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(2) };
	optionsSizer->AddGrowableCol(0);
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT | wxLEFT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);

	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("Maximum iterations:")) };
	optionsSizer->Add(label, labelFlags);
	maxIterations = new wxSpinCtrl(this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1000, 100);
	optionsSizer->Add(maxIterations, valueFlags);

	label = new wxStaticText(this, wxID_ANY, wxT("Early stopping"));
	optionsSizer->Add(label, labelFlags.Border(wxTOP, 10));
	optionsSizer->AddSpacer(0);

	useCorrelationThreshold = new wxCheckBox(this, wxID_ANY, wxT("Correlation higher than"));
	useCorrelationThreshold->SetValue(true);
	optionsSizer->Add(useCorrelationThreshold, labelFlags.Border(wxRIGHT | wxLEFT));
	correlationThreshold = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.99"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1, 0.99, 0.01);
	optionsSizer->Add(correlationThreshold, valueFlags);
	
	useRmseThreshold = new wxCheckBox(this, wxID_ANY, wxT("RMSE [st] less than"));
	useRmseThreshold->SetValue(true);
	optionsSizer->Add(useRmseThreshold, labelFlags);
	rmseThreshold = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1, 0.2, 0.01);
	optionsSizer->Add(rmseThreshold, valueFlags);
	
	this->SetSizer(optionsSizer);
}

OptimizerOptions OptimizerPage::getParameters()
{
	OptimizerOptions opts;
	opts.maxIterations = maxIterations->GetValue();
	opts.correlationThreshold = correlationThreshold->GetValue();
	opts.useCorrelationThreshold = useCorrelationThreshold->IsChecked();
	opts.rmseThreshold = rmseThreshold->GetValue();
	opts.useRmseThreshold = useRmseThreshold->IsChecked();

	return opts;
}

#endif
