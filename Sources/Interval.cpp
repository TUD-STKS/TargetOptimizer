#include "Interval.h"

Interval::Interval(double xmin, double xmax, std::string str)
{
	tmin = xmin;
	tmax = xmax;
	text = str;
}

Interval::Interval()
{
	tmin = 0;
	tmax = 0;
	text = "";
}

double Interval::getStart()
{
	return this->tmin;
}

double Interval::getEnd()
{
	return this->tmax;
}

std::string Interval::name()
{
	return this->text;
}

void Interval::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
}

void Interval::changeText(std::string text)
{
	this->text = text;
}
