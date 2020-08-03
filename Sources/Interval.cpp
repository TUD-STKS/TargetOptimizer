#include "Interval.h"

Interval::Interval(double tmin, double tmax, std::string text) : tmin(tmin), tmax(tmax), text(s)
{
	
}

double Interval::getStart()
{
	return this->tmin;
}

bool Interval::setStart(double newStart)
{
	if (newStart > this->tmax)
	{
		// TODO: Error handling
		return false;
	}
	this->tmin = newStart;	
	return true;
}

double Interval::getEnd()
{
	return this->tmax;
}

bool Interval::setEnd(double newEnd)
{
	if (newEnd < this->tmin)
	{
		//TODO: Error handling
		return false;
	}
	this->tmax = newEnd;
	return true;
	
}

void Interval::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
}
