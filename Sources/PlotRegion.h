#pragma once
#include <dlib/gui_widgets.h>
#include "dataio.h"

using namespace dlib;


class PlotRegion : public zoomable_region
{
public:
    PlotRegion(drawable_window& w,
        const BoundaryVector& bounds,
        const TargetVector& targets, const TimeSignal& optimalF0, const TimeSignal& originalF0);
    ~PlotRegion();

    void update();

private:
    void draw(const canvas& c) const;
    SignalStat analyzeSignal(const TimeSignal& f0) const;

    const TimeSignal& m_optF0;
    const TimeSignal& m_origF0;
    const BoundaryVector& m_boundaries;
    const TargetVector& m_targets;
};
