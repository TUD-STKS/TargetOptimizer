#ifdef USE_WXWIDGETS

#include "ResultPage.h"


ResultPage::ResultPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	wxFlexGridSizer* resultsSizer{ new wxFlexGridSizer(3) };
	resultsTable = new wxGrid(this, IDC_RESULT_TABLE);
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

void ResultPage::setEntries( std::vector<PitchTarget> pitchTargets )
{
	// Resize result table to correct number of columns
		int colDifference = pitchTargets.size() - resultsTable->GetNumberCols();
		if (colDifference > 0) { resultsTable->InsertCols(0, colDifference); }
		if (colDifference < 0) { resultsTable->DeleteCols(0, -1*colDifference); }

		int col = 0;
		for (const auto& target : pitchTargets)
		{
			resultsTable->SetColLabelValue(col, wxT("Target ") + wxString::Format(wxT("%i"), col));
			resultsTable->SetCellValue(wxGridCellCoords(0, col), wxString::Format(wxT("%.3f"), target.slope));
			resultsTable->SetCellValue(wxGridCellCoords(1, col), wxString::Format(wxT("%.3f"), target.offset));
			resultsTable->SetCellValue(wxGridCellCoords(2, col), wxString::Format(wxT("%.3f"), target.tau));
			resultsTable->SetCellValue(wxGridCellCoords(3, col), wxString::Format(wxT("%.3f"), target.duration));

			col++;
		}
}

#endif
