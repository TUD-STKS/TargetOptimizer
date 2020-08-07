#pragma once
#include <string>
#include <vector>
#include "TextGrid.h"

class TextGridReader {
public:
	// constructors
	TextGridReader(const std::string& textGridFile);

	// public member functions
	std::vector<double> getBounds(std::string tierName = "Position") const;
	
public:
	std::vector<std::string> getIntervalTierNames() const;

private:
	// data members
	TextGrid tg;
};