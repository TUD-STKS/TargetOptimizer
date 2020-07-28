#include <string>

class Interval {
public:
	// constructors
	Interval(double xmin, double xmax, std::string name = "");

	double start();
	double end();
	std::string name();
	Interval changeOffset(double offset);
	Interval changeText(std::string text);

private:
	double tmin;
	double tmax;
	std::string mark;
};