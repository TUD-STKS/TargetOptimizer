#include <wx/spinctrl.h>
#include "RegularizationPage.h"


RegularizationPage::RegularizationPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	// A sizer to organize the option labels and values
	wxFlexGridSizer* optionsSizer{ new wxFlexGridSizer(2) };
	optionsSizer->AddGrowableCol(0);
	wxSizerFlags labelFlags;
	labelFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT, 5);
	wxSizerFlags valueFlags;
	valueFlags.Align(wxLEFT | wxALIGN_CENTER_VERTICAL).Expand().Proportion(1).Border(wxALL, 5);

	wxStaticText* label{ new wxStaticText(this, wxID_ANY, wxT("lambda")) };
	optionsSizer->Add(label, labelFlags);
	wxSpinCtrlDouble* value{ new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.01"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT) };
	optionsSizer->Add(value, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("weight-slope"));
	optionsSizer->Add(label, labelFlags);	
	value = new wxSpinCtrlDouble(this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	optionsSizer->Add(value, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("weight-offset"));
	optionsSizer->Add(label, labelFlags);
	value = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	optionsSizer->Add(value, valueFlags);
	
	label = new wxStaticText(this, wxID_ANY, wxT("weight-tau"));
	optionsSizer->Add(label, labelFlags);
	value = new wxSpinCtrlDouble(this, wxID_ANY, wxT("0.1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	optionsSizer->Add(value, valueFlags);
	
	this->SetSizer(optionsSizer);
}
