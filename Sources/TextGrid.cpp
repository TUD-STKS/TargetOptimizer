#include <algorithm>
#include "TextGrid.h"


TextGrid::TextGrid(std::string name)
{
	mark = name;
	n = 0;
	tmin = 0;
	tmax = 0;
}

int TextGrid::getNumberOfTiers()
{
	return this->n;
}

IntervalTier TextGrid::getIntervalTier(int i)
{
	return this->intervalTiers[i];
}

PointTier TextGrid::getPointTier(int i)
{
	return this->pointTiers[i];
}

double TextGrid::start()
{
	return this->tmin;
}

double TextGrid::end()
{
	return this->tmax;
}

std::string TextGrid::name()
{
	return this->mark;
}

void TextGrid::append(IntervalTier intervalTier)
{
	this->intervalTiers.push_back(intervalTier);
	this->tmax = std::max(this->tmax, intervalTier.end());
	this->tmin = std::min(this->tmin, intervalTier.start());
	this->n += 1;
}

void TextGrid::append(PointTier pointTier)
{
	this->pointTiers.push_back(pointTier);
	this->tmax = std::max(this->tmax, pointTier.end());
	this->tmin = std::min(this->tmin, pointTier.start());
	this->n += 1;
}

void TextGrid::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
	for (auto&& intervalTier : this->intervalTiers) {
		intervalTier.changeOffset(offset);
	}
	for (auto&& pointTier : this->pointTiers) {
		pointTier.changeOffset(offset);
	}
}

void TextGrid::changeTimes(double start, double end)
{
	this->tmin = start;
	this->tmax = end;
}
