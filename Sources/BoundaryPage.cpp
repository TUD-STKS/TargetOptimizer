#ifdef USE_WXWIDGETS

#include "BoundaryPage.h"
#include <wx/wx.h>
int IDC_BOUNDARY_TABLE = wxNewId();

BoundaryPage::BoundaryPage(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{

	wxBoxSizer* boundarySizer{ new wxBoxSizer(wxVERTICAL) };
	boundaryTable = new wxGrid(this, IDC_BOUNDARY_TABLE);
	boundaryTable->SetLabelBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	boundaryTable->EnableEditing(true);
	boundaryTable->CreateGrid(1, 1);
	boundaryTable->SetRowLabelValue(0, wxT("Time"));

	boundaryTable->SetColLabelValue(0, wxEmptyString);
	boundaryTable->SetRowLabelSize(wxGRID_AUTOSIZE);
	boundaryTable->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
	boundaryTable->SetColLabelSize(wxGRID_AUTOSIZE);
	boundaryTable->SetColLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	boundaryTable->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
	boundarySizer->Add(boundaryTable, wxSizerFlags(1).Expand().Border(wxALL, 5));
	this->SetSizer(boundarySizer);
}

BoundaryParameters BoundaryPage::getParameters()
{
	BoundaryParameters params;

	return params;
}

void BoundaryPage::setEntries( BoundaryVector boundaries )
{
	// Resize boundary table to correct number of columns
		int colDifference = boundaries.size() - boundaryTable->GetNumberCols();
		if (colDifference > 0) { boundaryTable->InsertCols(0, colDifference ); }
		if (colDifference < 0) { boundaryTable->DeleteCols(0, -1*colDifference ); }

		int col = 0;

		for (const auto& boundary : boundaries)
		{
			boundaryTable->SetColLabelValue(col, wxT("Bound. ") + wxString::Format(wxT("%i"), col));
			boundaryTable->SetCellValue(wxGridCellCoords(0, col), wxString::Format(wxT("%.3f"), boundary));
			col++;
		}
}
#endif
