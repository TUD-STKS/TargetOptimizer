#pragma once
#include "BasicPicture.h"
#include "Graph.h"
#include "../dataio.h"

class wxPlotRegion :
	public BasicPicture
{
public:
	wxPlotRegion(wxWindow* parent, const BoundaryVector& boundaries, const TimeSignal& originalF0);

private:
	void draw(wxDC& dc) override;
	void drawBoundaries(wxDC& dc);
	void drawOriginalF0(wxDC& dc);
	const BoundaryVector& m_boundaries;
	const TimeSignal& m_origF0;

private:
	Graph plot;
};

