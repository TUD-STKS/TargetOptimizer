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

//bool Interval::setStart(double newStart)
//{
//	if (newStart > this->getEnd())
//		{
//			// TODO: Error handling
//			std::cout << "Starting time is later than the end of the selected interval!" << std::endl;
//			return false;
//		}
//	return true;
//}
//
//bool Interval::setEnd(double newEnd)
//{
//	if (newEnd < this->getStart())
//	{
//		std::cout << "Ending time is earlier than the start of the selected interval!" << std::endl;
//		return false;
//	}
//
//	return true;
//}


void Interval::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
}

