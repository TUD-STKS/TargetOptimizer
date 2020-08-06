#pragma once
#ifdef USE_WXWIDGETS

#include "BasicPicture.h"
#include "Graph.h"
#include "dataio.h"

class PlotRegion :
	public BasicPicture
{
public:
	PlotRegion(wxWindow* parent, 
		const BoundaryVector& boundaries, 
		const TimeSignal& originalF0, 
		const TargetVector& targets,
		const TimeSignal& optimalF0);

private:
	void draw(wxDC& dc) override;
	void drawBoundaries(wxDC& dc);
	void drawOptimalF0(wxDC& dc);
	void drawOriginalF0(wxDC& dc);
	void drawTargets(wxDC& dc);	
	void setAxesLimits();

private:
	const BoundaryVector& m_boundaries;
	const TimeSignal& m_origF0;
	const TimeSignal& m_optimalF0;
	const TargetVector& m_targets;
	Graph plot;
};

#endif // USE_WXWIDGETS