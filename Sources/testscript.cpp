#include <string>
#include "TextGrid.h"

int main() {

	TextGrid tg = TextGrid::readTextGridFile("De-Gabelschlüssel.TextGrid");

	TextGrid tg2 = tg; // copy constructor works

	IntervalTier intervalTierToAppend = IntervalTier("Appended (Type: Interval)"); // creating new interval tier
	intervalTierToAppend.append(Interval(0.2, 1.3, "New Interval")); // appending intervals
	intervalTierToAppend.append(Interval(1.3, 3.6, "New Interval"));
	//intervalTierToAppend.setConsecutive(false);
	intervalTierToAppend.append(Interval(3.8, 5, "New Interval")); // won't work as long the interval tier is set to "mustBeConsecutive = true"

	PointTier pointTierToAppend = PointTier("Appended (Type: Point)"); // creating new point tier
	pointTierToAppend.append(Point(0.2, "First Point")); // appending points
	pointTierToAppend.append(Point(2, "Second Point"));

	tg.appendIntervalTier(intervalTierToAppend); // appending interval tier
	tg.appendPointTier(pointTierToAppend); // appending point tier

	tg.getIntervalTier("Appended (Type: Interval)").setElementStart(0, 1); // setting new starting time
	tg.getIntervalTier("Appended (Type: Interval)").setElementEnd(4, 1.7); // bad index
	tg.getIntervalTier("Appended (Type: Interval)").setElementStart(1, 1.2); // setting new starting time of second element (end of first element is shifted accordingly)
	tg.getIntervalTier("Appended (Type: Interval)").setElementEnd(100, 5000); // bad index

	IntervalTier intervalTierToGet = tg.getIntervalTier("Appended (Type: Interval)"); // getting interval tiers
	PointTier pointTierToGet = tg.getPointTier("Appended (Type: Point)"); // getting point tiers

	Interval intervalToGet = tg.getIntervalTier("Appended (Type: Interval)")[0]; // getting intervals in selected interval tier with overloaded []-operator
	Point pointToGet = tg.getPointTier("Appended (Type: Point)")[1]; // getting points in selected point tier with overloaded []-operator

	double minimumTG = tg.getStart(); // getting starting time of text grid
	double maximumTG = tg.getEnd(); // getting ending time of text grid

	double minimumIT = tg.getIntervalTier("Appended (Type: Interval)").getStartingTime(); // getting starting time of selected interval
	double maximumIT = tg.getIntervalTier("Appended (Type: Interval)").getEndingTime(); // getting ending time of selected interval

	TextGrid tg3 = tg; // copy constructor works even after manipulation

	TextGrid::writeTextGridFile(tg, "test_short.TextGrid", "short");
	TextGrid::writeTextGridFile(tg, "test_long.TextGrid", "long");

	return 0;
}