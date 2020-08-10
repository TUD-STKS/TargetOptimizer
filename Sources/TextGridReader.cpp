#include <fstream>
#include <dlib/string.h>
#include <dlib/error.h>
#include <iostream>
#include "TextGridReader.h"


TextGridReader::TextGridReader(const std::string& textGridFile)
{
	tg = TextGrid::readTextGridFile(textGridFile);
}

std::vector<double> TextGridReader::getBounds(std::string tierName) const
{
	IntervalTier boundsTier;
	auto it = tg.intervalTiers.find(tierName);
	if (it == tg.intervalTiers.end())
	{
		throw std::runtime_error("Could not find an interval tier named " + tierName + "!");
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
