#pragma once
#include <string>
#include <vector>
#include <map>
#include "Interval.h"
#include "Point.h"

typedef Tier<Interval>	IntervalTier;
typedef Tier<Point>		PointTier;


class TextGrid
{
public:

	TextGrid() = default;

	static TextGrid readTextGridFile(const std::string& inputFilename);
	static bool writeTextGridFile(TextGrid& tg, const std::string& outputFilename, const std::string& format);

	void appendIntervalTier(IntervalTier& intervalTier);
	void appendPointTier(PointTier& pointTier);

	IntervalTier& getIntervalTier(std::string name);
	PointTier& getPointTier(std::string name);

	double getStart();
	double getEnd();	

public:
	std::multimap<std::string, IntervalTier> intervalTiers;
	std::multimap<std::string, PointTier> pointTiers;

private:
	static TextGrid BinaryTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);
	static TextGrid ShortTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);
	static TextGrid LongTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);	
};

template<class TierType>
inline bool sortElements(Tier<TierType>& tier)
{
	return false;
}
