#include <iostream>
#include "Point.h"

Point::Point(double number, std::string mark, bool isConsecutive) : number(number), mark(mark), isConsecutive(isConsecutive)
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

bool Point::getConsecutiveness()
{
	return isConsecutive;
}

