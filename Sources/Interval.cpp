#include <iostream>
#include "Interval.h"

Interval::Interval(double tmin, double tmax, std::string text) : tmin(tmin), tmax(tmax), text(text)
{
	
}

double Interval::getStart()
{
	return this->tmin;
}

double Interval::getEnd()
{
	return this->tmax;
}


void Interval::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
}

