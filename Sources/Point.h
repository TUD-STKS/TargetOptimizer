#include <string>

class Point
{
public:
	Point(double number, std::string mark, bool isConsecutive = false);

	double getStart();
	double getEnd();
	bool getConsecutiveness();

public:
	double number;
	std::string mark;

private:
	bool isConsecutive;
};

