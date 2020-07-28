#include "Interval.h"

Interval::Interval(double xmin, double xmax, std::string name)
{
	tmin = xmin;
	tmax = xmax;
	mark = name;
}

double Interval::start()
{
	return this->tmin;
}

double Interval::end()
{
	return this->tmax;
}

std::string Interval::name()
{
	return this->mark;
}

Interval Interval::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
	return *this;
}

Interval Interval::changeText(std::string text)
{
	this->mark = text;
	return *this;
}
