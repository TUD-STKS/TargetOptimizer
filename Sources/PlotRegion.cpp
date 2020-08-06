#ifdef USE_WXWIDGETS
#include <algorithm>
#include "PlotRegion.h"

PlotRegion::PlotRegion(wxWindow* parent, const BoundaryVector& bounds, const TimeSignal& originalF0, const TargetVector& targets, const TimeSignal& optimalF0) :
    BasicPicture(parent), 
    m_boundaries(bounds), m_origF0(originalF0), m_targets(targets), m_optimalF0(optimalF0)
{
    // Plot needs some margin to paint the axes and labels
    plot.init(this, 60, 0, 0, 40);
    plot.initAbscissa(PQ_TIME, 0.0, 0.01,
        0.0, 0.0, 0.0, 0.1, 0.1, 0.1,
        1, 1, true, true, true);
    plot.initLinearOrdinate(PQ_PITCH, 0.0, 0.01,
        0.0, 0.0, 50.0, 0.1, 0.1, 100,
        1, 1, true, true, true);
    }


void PlotRegion::draw(wxDC& dc)
{
    // Fill the background
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    // Set the axes limits
    setAxesLimits();

    // Draw the syllable boundaries
    drawBoundaries(dc);
    // Draw the F0 pitch marks
    drawOriginalF0(dc);
    // Draw the pitch targets
    drawTargets(dc);
    // Draw F0 as produced by the Target Approximation Model
    drawOptimalF0(dc);

    // Paint the axes
    plot.paintAbscissa(dc);
    plot.paintOrdinate(dc);

    
 }

void PlotRegion::drawBoundaries(wxDC& dc)
{
    int maxX, maxY, height, width;
    plot.getDimensions(maxX, maxY, height, width);

    dc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_DOT));

    if (!m_boundaries.empty())
    {  
        for (const auto& boundary : m_boundaries)
        {
            wxPoint p0, p1;
            p0.x = plot.getXPos(boundary);
            p0.y = 0;

            p1.x = plot.getXPos(boundary);
            p1.y = p0.y + height;

            dc.DrawLine(p0, p1);
        }
    }    
}

void PlotRegion::drawOptimalF0(wxDC& dc)
{
    dc.SetPen(wxPen(*wxGREEN, 5, wxPENSTYLE_SOLID));
    if (!m_optimalF0.empty())
    {
        int radius = 2;
        for (const auto& pitchMark : m_optimalF0)
        {
            wxPoint p;
            p.x = plot.getXPos(pitchMark.time);
            p.y = plot.getYPos(pitchMark.value);
            dc.DrawCircle(p, radius);
        }
    }
}

void PlotRegion::drawOriginalF0(wxDC& dc)
{
    int maxX, maxY, height, width;
    plot.getDimensions(maxX, maxY, height, width);

    dc.SetPen(wxPen(*wxBLUE, 5, wxPENSTYLE_SOLID));
    int radius = 2;
    if (!m_origF0.empty())
    {
        for (const auto& pitchMark : m_origF0)
        {
            wxPoint p;
            p.x = plot.getXPos(pitchMark.time);
            p.y = plot.getYPos(pitchMark.value);
            dc.DrawCircle(p, radius);
        }
    }    
}

void PlotRegion::drawTargets(wxDC& dc)
{
    if (!m_targets.empty())
    {
        dc.SetPen(wxPen(*wxBLACK, 3, wxPENSTYLE_LONG_DASH));
        // Draw the targets as lines from one boundary to the next one
        double begin = *std::min(m_boundaries.begin(), m_boundaries.end());
        double end = begin;
        wxPoint p0, p1;
        for (const auto& target : m_targets)
        {
            p0.x = plot.getXPos(begin);
            p0.y = plot.getYPos(target.offset);
            end = begin + target.duration;
            p1.x = plot.getXPos(end);
            p1.y = plot.getYPos(target.slope * (end - begin) + target.offset);
            begin = end;
            dc.DrawLine(p0, p1);
        }
    }
}

void PlotRegion::setAxesLimits()
{
    
    /* Determine the largest data point to be plotted */
    double maxTimeBoundary = 0.1;
    double minTimeBoundary = 0.0;
    if (!m_boundaries.empty())
    {
        // Largest boundary
        maxTimeBoundary = *std::max_element(m_boundaries.begin(), m_boundaries.end());
        // Smallest boundary
        minTimeBoundary = *std::min_element(m_boundaries.begin(), m_boundaries.end());
    }
    
    double maxTimeOriginalF0 = 0.1;
    double minTimeOriginalF0 = minTimeBoundary;
    double maxOriginalF0 = 1;
    double minOriginalF0 = 0;
    if (!m_origF0.empty())
    {
        // Largest time instant of original F0
        maxTimeOriginalF0 = std::max_element(m_origF0.begin(),
            m_origF0.end(),
            [](const auto& a, const auto& b) { return a.time < b.time; })->time;
        // Smallest time instant of original F0
        minTimeOriginalF0 = std::min_element(m_origF0.begin(),
            m_origF0.end(),
            [](const auto& a, const auto& b) { return a.time < b.time; })->time;
        // Largest F0 value
        maxOriginalF0 = std::max_element(m_origF0.begin(),
            m_origF0.end(),
            [](const auto& a, const auto& b) { return a.value < b.value; })->value;
        // Smallest F0 value
        minOriginalF0 = std::min_element(m_origF0.begin(),
            m_origF0.end(),
            [](const auto& a, const auto& b) { return a.value < b.value; })->value;
    }
    
    double maxTimeOptimalF0 = 0.1;
    double minTimeOptimalF0 = minTimeOriginalF0;
    double maxOptimalF0 = 1;
    double minOptimalF0 = minOriginalF0;
    if (!m_optimalF0.empty())
    {
        // Largest time instant of optimal F0
        maxTimeOptimalF0 = std::max_element(m_optimalF0.begin(),
            m_optimalF0.end(),
            [](const auto& a, const auto& b) { return a.time < b.time; })->time;
        // Smallest time instant of optimal F0
        minTimeOptimalF0 = std::min_element(m_optimalF0.begin(),
            m_optimalF0.end(),
            [](const auto& a, const auto& b) { return a.time < b.time; })->time;
        // Largest F0 value
        maxOptimalF0 = std::max_element(m_optimalF0.begin(),
            m_optimalF0.end(),
            [](const auto& a, const auto& b) { return a.value < b.value; })->value;
        // Smallest F0 value
        minOptimalF0 = std::min_element(m_optimalF0.begin(),
            m_optimalF0.end(),
            [](const auto& a, const auto& b) { return a.value < b.value; })->value;
    }
    
    /* Set the limits */
    plot.abscissa.positiveLimit = std::max({ maxTimeBoundary, maxTimeOriginalF0, maxTimeOptimalF0 }) + 0.1;
    plot.abscissa.negativeLimit = std::min({ minTimeBoundary, minTimeOriginalF0, minTimeOptimalF0 }) - 0.1;
    plot.linearOrdinate.positiveLimit = std::max({ maxOriginalF0, maxOptimalF0 }) + 6;
    plot.linearOrdinate.negativeLimit = std::min({ minOriginalF0, minOptimalF0 }) - 6;
}

#endif // USE_WXWIDGETS
