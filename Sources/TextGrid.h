#include <string>
#include <vector>
#include <variant>
#include "Tier.h"

typedef Tier<Interval>	IntervalTier;
typedef Tier<Point>		PointTier;

class TextGrid
{
public:
	TextGrid() = default;

	static TextGrid readTextGridFile(const std::string& inputFilename);
	static bool writeTextGridFile(const TextGrid& tg, const std::string& outputFilename, const std::string& format);

	double getStart();
	double getEnd();

public:
	std::vector<IntervalTier> intervalTiers;
	std::vector<PointTier> pointTiers;

private:
	static TextGrid BinaryTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);
	static TextGrid ShortTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);
	static TextGrid LongTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);		
};
