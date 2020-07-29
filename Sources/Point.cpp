#include "Point.h"

Point::Point(double time, double value)
{
	x = time;
	y = value;
}

double Point::time()
{
	return this->x;
}

double Point::mark()
{
	return this->y;
}

void Point::changeOffset(double offset)
{
	this->x += offset;
}
