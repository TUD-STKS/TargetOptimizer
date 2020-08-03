#include <iostream>
#include "Point.h"

Point::Point(double number, std::string mark) : number(number), mark(mark)
{
	
}

double Point::getStart()
{
	return this->number;
}

double Point::getEnd()
{
	return this->number;
}

