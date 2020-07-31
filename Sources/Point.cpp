#include <iostream>
#include "Point.h"

Point::Point(double time, std::string label)
{
	number = time;
	mark = label;
}

double Point::getStart()
{
	return this->number;
}

double Point::getEnd()
{
	return this->number;
}

std::string Point::getMark()
{
	return this->mark;
}

void Point::changeOffset(double offset)
{
	this->number += offset;
}

void Point::changeText(std::string newText)
{
	this->mark = newText;
}
