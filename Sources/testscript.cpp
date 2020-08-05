#include "TextGrid.h"
#include <string>

int main() {

	TextGrid tg = TextGrid::readTextGridFile(R"(C:\Users\stone\source\repos\TargetOptimizer\Sources\De-Gabelschlüssel_Long.TextGrid)");

	TextGrid tg2 = tg; // copy constructor works

	IntervalTier intervalTierToAppend = IntervalTier("Appended (Type: Interval)"); // creating new interval tier
	intervalTierToAppend.append(Interval(1.3, 3.6, "New Interval")); // appending intervals
	intervalTierToAppend.append(Interval(0.2, 1.3, "New Interval (2)"));

	intervalTierToAppend.setElementStart(0, 0.0);
	
	PointTier pointTierToAppend = PointTier("Appended (Type: Point)"); // creating new point tier
	pointTierToAppend.append(Point(0.2, "First Point")); // appending points
	pointTierToAppend.append(Point(1000, "Second Point"));

	tg.appendIntervalTier(intervalTierToAppend); // appending interval tier
	tg.appendPointTier(pointTierToAppend); // appending point tier

	IntervalTier intervalTierToGet = tg.getIntervalTier("Appended (Type: Interval)"); // getting interval tiers
	PointTier pointTierToGet = tg.getPointTier("Appended (Type: Point)"); // getting point tiers

	Interval intervalToGet = tg.getIntervalTier("Appended (Type: Interval)")[0]; // getting intervals in selected interval tier with overloaded []-operator
	Point pointToGet = tg.getPointTier("Appended (Type: Point)")[1]; // getting points in selected point tier with overloaded []-operator

	double minimumTG = tg.getStart(); // getting starting time of text grid
	double maximumTG = tg.getEnd(); // getting ending time of text grid

	double minimumIT = tg.getIntervalTier("Appended (Type: Interval)").getStartingTime(); // getting starting time of selected interval
	double maximumIT = tg.getIntervalTier("Appended (Type: Interval)").getEndingTime(); // getting ending time of selected interval

	TextGrid tg3 = tg; // copy constructer works even after manipulation

	return 0;
}