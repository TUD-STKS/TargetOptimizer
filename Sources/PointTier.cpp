#include <algorithm>
#include "PointTier.h"


PointTier::PointTier(double xmin, double xmax, std::string name)
{
	tmin = xmin;
	tmax = xmax;
	mark = name;
	n = 0;
}

int PointTier::getNumberOfPoints()
{
	return this->n;
}

Point PointTier::getItem(int i)
{
	return this->points[i];
}

double PointTier::start()
{
	return this->tmin;
}

double PointTier::end()
{
	return this->tmax;
}

std::string PointTier::name()
{
	return this->mark;
}

PointTier PointTier::append(Point point)
{
	this->points.push_back(point);
	this->tmax = std::max(point.time(), this->tmax);
	this->tmin = std::min(point.time(), this->tmin);
	this->n = this->getNumberOfPoints();
	return *this;
}
