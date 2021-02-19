#pragma once
#ifdef USE_WXWIDGETS

#include "BasicPicture.h"
#include "Graph.h"
#include "Data.h"


class PlotRegion :
	public BasicPicture
{
public:
	PlotRegion(wxWindow* parent, 
		const BoundaryVector& initialBoundaries, 
		const TimeSignal& originalF0, 
		const BoundaryVector& optimalBoundaries, 
		const TargetVector& targets, 
		const TimeSignal& optimalF0);

private:
	void draw(wxDC& dc) override;
	void drawInitialBoundaries(wxDC& dc);
	void drawOptimalBoundaries(wxDC& dc);
	void drawOptimalF0(wxDC& dc);	
	void drawOriginalF0(wxDC& dc);
	void drawTargets(wxDC& dc);	
	void setAxesLimits();

private:
	const BoundaryVector& m_initialBoundaries;
	const TimeSignal& m_origF0;
	const BoundaryVector& m_optimalBoundaries;
	const TimeSignal& m_optimalF0;
	const TargetVector& m_targets;
	Graph plot;
};

#endif // USE_WXWIDGETS