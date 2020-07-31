#include <string>
#include <vector>
#include <variant>
#include "Tier.h"

class TextGrid
{
public:
	TextGrid(double xmin, double xmax, int nTiers);

	static TextGrid readTextGridFile(const std::string& inputFilename);
	static bool writeTextGridFile(const TextGrid& tg, const std::string& outputFilename, const std::string& format);

			
	auto getTier(int index) { return tierElements[index]; };

	int getNumberOfTiers();
	void append(IntervalTier intervalTier);
	void append(PointTier pointTier);

	auto operator[](int index) { return getTier(index); };

private:
	static TextGrid BinaryTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);
	static TextGrid ShortTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);
	static TextGrid LongTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements);		
	

	double tmin;
	double tmax;
	int numberOfTiers;


	std::vector<std::variant<IntervalTier, PointTier>> tierElements;
};
