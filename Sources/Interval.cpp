#include <iostream>
#include "Interval.h"

Interval::Interval(double tmin, double tmax, std::string text, bool isConsecutive) : tmin(tmin), tmax(tmax), text(text), isConsecutive(isConsecutive)
{
	
}

double Interval::getStart() const
{
	return this->tmin;
}

double Interval::getEnd() const
{
	return this->tmax;
}


void Interval::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
}

bool Interval::getConsecutiveness()
{
	return isConsecutive;
}

