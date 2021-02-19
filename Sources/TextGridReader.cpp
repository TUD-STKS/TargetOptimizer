#include <fstream>
#include <dlib/string.h>
#include <dlib/error.h>
#include <iostream>
#include "TextGridReader.h"

#if defined(__GNUC__)
// The GCC considers the C++17 filesystem header experimental
#include <experimental/filesystem>
using namespace std::experimental::filesystem;
#else
#include <filesystem>
using namespace std::filesystem;
#endif


TextGridReader::TextGridReader(const std::string& textGridFile)
{
	tg = TextGrid::readTextGridFile(textGridFile);

	if (tg.intervalTiers.empty() && tg.pointTiers.empty())
	{
		throw std::runtime_error("[read_data_file] TextGrid input file not found or corrupted!");
	}
}

std::vector<double> TextGridReader::getBounds() const
{
	IntervalTier boundsTier;
	auto it = tg.intervalTiers.find(syllableBoundaryTierName);
	if (it == tg.intervalTiers.end())
	{
		throw std::runtime_error("Could not find an interval tier named " + syllableBoundaryTierName + "!");
	}
	boundsTier = it->second;
	std::vector<double> bounds;
	for (const auto& boundInterval : boundsTier)
	{	
		if (boundInterval.text == "" && bounds.empty())
		{
			// This is the first interval (which holds silence)
			// The end of this interval is the initial start of the first target
			bounds.push_back(boundInterval.getEnd());
		}
		else if (boundInterval.text == "")
		{
			// This is the last interval (which holds silence)
			// Do nothing
		}
		else
		{
			// Add the next initial target boundary
			bounds.push_back(boundInterval.getEnd());
		}		
	}
	return bounds;
}

std::vector<std::string> TextGridReader::getIntervalTierNames() const
{
	std::vector<std::string> intervalTierNames;
	for (const auto& intervalTier : tg.intervalTiers)
	{
		intervalTierNames.push_back(intervalTier.first);
	}
	return intervalTierNames;
}
