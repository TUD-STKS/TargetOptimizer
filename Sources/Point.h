#include <string>

class Point
{
public:
	Point(double number, std::string mark);

	double getStart();
	double getEnd();

public:
	double number;
	std::string mark;
};

