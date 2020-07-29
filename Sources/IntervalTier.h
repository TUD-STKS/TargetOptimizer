#include <string>
#include <vector>
#include "Interval.h"

class IntervalTier {
public:
	// constructor
	IntervalTier(double xmin = 0, double xmax = 0, std::string name = "");

	int getNumberOfIntervals();
	Interval getItem(int i);
	double start();
	double end();
	std::string name();
	void append(Interval interval);
	void rename(std::string newName);
	void changeOffset(double offset);


private:
	double tmin;
	double tmax;
	std::string mark;
	int n;
	std::vector<Interval> intervals;
};
