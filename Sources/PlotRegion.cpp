#include "PlotRegion.h"

PlotRegion::PlotRegion(drawable_window& w, const BoundaryVector& boundaries,
	const TargetVector& targets, const TimeSignal& optimalF0, const TimeSignal& originalF0) :
	m_boundaries(boundaries), m_targets(targets), m_optF0(optimalF0), m_origF0(originalF0),
	zoomable_region(w, MOUSE_CLICK | MOUSE_WHEEL | KEYBOARD_EVENTS)
{
	enable_events();
}

PlotRegion::~PlotRegion()
{
	disable_events();
	parent.invalidate_rectangle(rect);
}

void PlotRegion::update()
{
	parent.invalidate_rectangle(rect);
}

void PlotRegion::draw(const canvas& c) const
{
	zoomable_region::draw(c);

	rectangle area = c.intersect(display_rect());
	if (area.is_empty() == true)
		return;

	if (enabled)
		fill_rect(c, display_rect(), 255);
	else
		fill_rect(c, display_rect(), 128);

	SignalStat scaler = { 0,0,0,0 };
	// draw boundaries
	if (!m_boundaries.empty())
	{
		scaler.minTime = m_boundaries[0] - 0.025;
		scaler.maxTime = m_boundaries[m_boundaries.size() - 1] + 0.05;
		for (unsigned i = 0; i < m_boundaries.size(); ++i)
		{
			double position = ((m_boundaries[i] - scaler.minTime) / (scaler.maxTime - scaler.minTime)) * width();
			point p(graph_to_gui_space(point(position, 0)));
			point p2(graph_to_gui_space(point(position, height())));
			draw_line(c, p2, p, rgb_pixel(0, 0, 0), area);
		}
	}

	// draw original f0
	if (!m_origF0.empty())
	{
		SignalStat stat = analyzeSignal(m_origF0);
		scaler.maxValue = stat.maxValue + 5;
		scaler.minValue = stat.minValue - 5;
		if (scaler.maxTime == 0.0)
		{
			scaler.maxTime = stat.maxTime + 0.05;
			scaler.minTime = stat.minTime - 0.025;
		}
		for (unsigned i = 0; i < m_origF0.size(); ++i)
		{
			double x = ((m_origF0[i].time - scaler.minTime) / (scaler.maxTime - scaler.minTime)) * width();
			double y = height() - ((m_origF0[i].value - scaler.minValue) / (scaler.maxValue - scaler.minValue)) * height();
			draw_solid_circle(c, graph_to_gui_space(point(x, y)), 2, rgb_pixel(0, 0, 255));
		}
	}

	// draw optimal f0
	if (!m_optF0.empty())
	{
		for (unsigned i = 0; i < m_optF0.size(); ++i)
		{
			double x = ((m_optF0[i].time - scaler.minTime) / (scaler.maxTime - scaler.minTime)) * width();
			double y = height() - ((m_optF0[i].value - scaler.minValue) / (scaler.maxValue - scaler.minValue)) * height();

			if (y >= 0 && y <= height() - 2)
			{
				draw_solid_circle(c, graph_to_gui_space(point(x, y)), 1.5, rgb_pixel(100, 200, 100));
			}
		}
	}

	// draw optimal targets
	if (!m_targets.empty())
	{
		double begin = m_optF0[0].time;
		double end = begin;
		for (unsigned i = 0; i < m_targets.size(); ++i)
		{
			begin = end;
			end = begin + m_targets[i].duration;
			double x1 = ((begin - scaler.minTime) / (scaler.maxTime - scaler.minTime)) * width();
			double y1 = height() - ((m_targets[i].offset - scaler.minValue) / (scaler.maxValue - scaler.minValue)) * height();
			double x2 = ((end - scaler.minTime) / (scaler.maxTime - scaler.minTime)) * width();
			double y2 = height() - (((m_targets[i].offset + m_targets[i].slope * m_targets[i].duration) - scaler.minValue) / (scaler.maxValue - scaler.minValue)) * height();
			draw_line(c, graph_to_gui_space(point(x1, y1)), graph_to_gui_space(point(x2, y2)), rgb_pixel(200, 100, 100), area);
		}
	}
}

SignalStat PlotRegion::analyzeSignal(const TimeSignal& f0) const
{
	std::vector<double> times, values;
	for (unsigned i = 0; i < f0.size(); ++i)
	{
		times.push_back(f0[i].time);
		values.push_back(f0[i].value);
	}

	SignalStat result;
	result.minTime = *std::min_element(times.begin(), times.end());
	result.maxTime = *std::max_element(times.begin(), times.end());
	result.minValue = *std::min_element(values.begin(), values.end());
	result.maxValue = *std::max_element(values.begin(), values.end());

	return result;
}