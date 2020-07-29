#include <algorithm>
#include <iostream>
#include <fstream>
#include "IntervalTier.h"

IntervalTier::IntervalTier(double xmin, double xmax, std::string name)
{
	tmin = xmin;
	tmax = xmax;
	mark = name;
	n = 0;
}

int IntervalTier::getNumberOfIntervals()
{
	return this->n;
}

Interval IntervalTier::getItem(int i)
{
	return this->intervals[i];
}

double IntervalTier::start()
{
	return this->tmin;
}

double IntervalTier::end()
{
	return this->tmax;
}

std::string IntervalTier::name()
{
	return this->mark;
}

void IntervalTier::append(Interval interval)
{
	this->intervals.push_back(interval);
	this->tmax = std::max(interval.end(), this->tmax);
	this->tmin = std::min(interval.start(), this->tmin);
	this->n = getNumberOfIntervals();
}

void IntervalTier::rename(std::string newName)
{
	this->mark = newName;
}

void IntervalTier::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
	for (auto&& interval:intervals) {
		interval.changeOffset(offset);
	}
}
