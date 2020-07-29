#include <string>
#include <regex>
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

	void textGridReader(std::string inputFilename);
	void textGridWriter(std::string outputFilename);

private:
	double tmin;
	double tmax;
	int n;
	std::string mark;
	std::vector<IntervalTier> intervalTiers;
	std::vector<PointTier> pointTiers;
	std::string textGridFormat;

	std::string ltrim(const std::string& s) {
		return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
	}
	std::string rtrim(const std::string& s) {
		return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
	}
	std::string trim(const std::string& s) {
		return ltrim(rtrim(s));
	}

	std::vector<std::string> split(std::string stringFromLine);
};

