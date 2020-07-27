#include "wxPlotRegion.h"

wxPlotRegion::wxPlotRegion(wxWindow* parent, const BoundaryVector& bounds, const TimeSignal& originalF0) : 
    BasicPicture(parent), 
    m_boundaries(bounds), m_origF0(originalF0)
{
    // Plot needs some margin to paint the axes and labels
    plot.init(this, 40, 0, 0, 25);
    plot.initAbscissa(PQ_TIME, 0.0, 0.01,
        0.0, 0.0, 0.0, 0.1, 0.1, 0.1,
        1, 1, true, true, true);
    plot.initLinearOrdinate(PQ_PITCH, 0.0, 0.01,
        0.0, 0.0, 50.0, 0.1, 0.1, 100,
        1, 1, true, true, true);
    }


void wxPlotRegion::draw(wxDC& dc)
{
    // ****************************************************************
    // Fill the background.
    // ****************************************************************
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();


    drawBoundaries(dc);

    drawOriginalF0(dc);


    // ****************************************************************
    // Paint the axes.
    // ****************************************************************
    plot.paintAbscissa(dc);
    plot.paintOrdinate(dc);

    
 }

void wxPlotRegion::drawBoundaries(wxDC& dc)
{
    int maxX, maxY, height, width;
    plot.getDimensions(maxX, maxY, height, width);

    dc.SetPen(wxPen(*wxRED, 3, wxPENSTYLE_DOT));

    if (m_boundaries.size() > 0)
    {
        /* Set the plot dimensions according to the boundaries */
        auto maxTime = *std::max_element(m_boundaries.begin(), m_boundaries.end());
        plot.abscissa.positiveLimit = std::max(maxTime + 0.1, plot.abscissa.positiveLimit);
        plot.abscissa.positiveLimit += 0.01;
        
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

void wxPlotRegion::drawOriginalF0(wxDC& dc)
{
    int maxX, maxY, height, width;
    plot.getDimensions(maxX, maxY, height, width);

    dc.SetPen(wxPen(*wxBLUE, 5, wxPENSTYLE_SOLID));
    int radius = 2;
    if (m_origF0.size() > 0)
    {
        /* Set the plot dimensions according to the boundaries */
        plot.linearOrdinate.positiveLimit = std::max_element(m_origF0.begin(), 
            m_origF0.end(), 
            [](const auto& a, const auto& b) { return a.value < b.value; })->value;
        plot.linearOrdinate.positiveLimit += 6;
        plot.linearOrdinate.negativeLimit = std::max_element(m_origF0.begin(),
            m_origF0.end(),
            [](const auto& a, const auto& b) { return a.value > b.value; })->value;
        plot.linearOrdinate.negativeLimit -= 6;
        auto maxTime = std::max_element(m_origF0.begin(),
            m_origF0.end(),
            [](const auto& a, const auto& b) { return a.time < b.time; })->time;
        plot.abscissa.positiveLimit = std::max(maxTime + 0.1 , plot.abscissa.positiveLimit);

        for (const auto& pitchMark : m_origF0)
        {
            wxPoint p;
            p.x = plot.getXPos(pitchMark.time);
            p.y = plot.getYPos(pitchMark.value);
            dc.DrawCircle(p, radius);
        }
    }
    
}

