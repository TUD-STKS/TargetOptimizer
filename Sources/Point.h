#include <string>

class Point
{
public:
	Point(double time, std::string label);

	double getStart();
	double getEnd();
	std::string getMark();
	void changeOffset(double offset);
	void changeText(std::string newText);

private:
	double number;
	std::string mark;
};

