#include <string>
#include "IntervalTier.h"
#include "PointTier.h"

class TextGrid
{
public:
	// constructors
	TextGrid(std::string name = "");

	int getNumberOfTiers();
	IntervalTier getIntervalTier(int i);
	PointTier getPointTier(int i);
	double start();
	double end();
	std::string name();
	void append(IntervalTier intervalTier);
	void append(PointTier pointTier);
	void changeOffset(double offset);
	void changeTimes(double start, double end);

private:
	double tmin;
	double tmax;
	int n;
	std::string mark;
	std::vector<IntervalTier> intervalTiers;
	std::vector<PointTier> pointTiers;

};

