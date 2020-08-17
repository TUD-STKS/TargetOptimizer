#ifdef USE_WXWIDGETS
#include "OptimizerPage.h"

static const int ID_EARLY_STOPPING_OPTION_VALUE = wxNewId();

wxBEGIN_EVENT_TABLE(OptimizerPage, wxPanel)
EVT_SPINCTRL(ID_EARLY_STOPPING_OPTION_VALUE, OnChangeValue)
EVT_SPINCTRLDOUBLE(ID_EARLY_STOPPING_OPTION_VALUE, OnChangeValue)
wxEND_EVENT_TABLE()

OptimizerPage::OptimizerPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// Get the default values of the OptimizerOptions
	OptimizerOptions defaultOptions;

	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(2) };
	optionsSizer->AddGrowableCol(0);
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT | wxLEFT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);

	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("Maximum iterations:")) };
	optionsSizer->Add(label, labelFlags);
	maxIterations = new wxSpinCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), defaultOptions.maxIterations), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1000, defaultOptions.maxIterations);
	optionsSizer->Add(maxIterations, valueFlags);

	label = new wxStaticText(this, wxID_ANY, wxT("Maximum cost function\nevaluations(per iteration):"));
	optionsSizer->Add(label, labelFlags);
	maxCostEvaluations = new wxSpinCtrl(this, wxID_ANY, wxString::Format(wxT("%i"), defaultOptions.maxCostEvaluations), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1000000, defaultOptions.maxCostEvaluations);
	optionsSizer->Add(maxCostEvaluations, valueFlags);

	label = new wxStaticText(this, wxID_ANY, wxT("Rho end (precision):"));
	optionsSizer->Add(label, labelFlags);
	rhoEnd = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.6f"), defaultOptions.rhoEnd), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1, defaultOptions.rhoEnd, 0.001);
	optionsSizer->Add(rhoEnd, valueFlags);
	
	useEarlyStopping = new wxCheckBox(this, wxID_ANY, wxT("Use early stopping"));
	useEarlyStopping->SetValue(defaultOptions.useEarlyStopping);
	optionsSizer->Add(useEarlyStopping, labelFlags.Border(wxTOP, 10));
	optionsSizer->AddSpacer(0);

	label = new wxStaticText(this, wxID_ANY, wxT("Optimization cost \U0001D700:"));
	optionsSizer->Add(label, labelFlags.Border(wxRIGHT | wxLEFT, 10));
	epsilon = new wxSpinCtrlDouble(this, ID_EARLY_STOPPING_OPTION_VALUE, wxString::Format(wxT("%.2f"), defaultOptions.epsilon), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, 1, defaultOptions.epsilon, 0.001);
	optionsSizer->Add(epsilon, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("Patience"));
	optionsSizer->Add(label, labelFlags);
	
	patience = new wxSpinCtrl(this, ID_EARLY_STOPPING_OPTION_VALUE, wxString::Format(wxT("%i"), defaultOptions.patience), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxSP_ARROW_KEYS, 0, maxIterations->GetValue(), defaultOptions.patience);
	optionsSizer->Add(patience, valueFlags);
	
	this->SetSizer(optionsSizer);
}

OptimizerOptions OptimizerPage::getParameters() const
{
	OptimizerOptions opts;
	opts.maxIterations = maxIterations->GetValue();
	opts.maxCostEvaluations = maxCostEvaluations->GetValue();
	opts.rhoEnd = rhoEnd->GetValue();
	opts.useEarlyStopping = useEarlyStopping->IsChecked(); 
	opts.epsilon = epsilon->GetValue();
	opts.patience = patience->GetValue();
	
	return opts;
}

void OptimizerPage::OnChangeValue(wxSpinEvent& event)
{
	useEarlyStopping->SetValue(true);
}

void OptimizerPage::OnChangeValue(wxSpinDoubleEvent& event)
{
	useEarlyStopping->SetValue(true);
}

#endif // USE_WXWIDGETS
