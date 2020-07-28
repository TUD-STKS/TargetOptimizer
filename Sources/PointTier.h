#include <string>
#include <vector>
#include "Point.h"

class PointTier
{
public:
	// constructor
	PointTier(double xmin = 0, double xmax = 0, std::string name = "");

	int getNumberOfPoints();
	Point getItem(int i);
	double start();
	double end();
	std::string name();
	PointTier append(Point point);


private:
	double tmin;
	double tmax;
	std::string mark;
	int n;
	std::vector<Point> points;
};
