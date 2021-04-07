#pragma once
#include <string>
#include <vector>
#include "TextGrid.h"

class TextGridReader {
public:
	// constructors
	TextGridReader() = default;
	TextGridReader(const std::string& textGridFile);

	// public member functions
	std::vector<double> getBounds() const;
	
public:
	std::vector<std::string> getIntervalTierNames() const;
	std::string syllableBoundaryTierName{ "Position" };

private:
	// data members
	TextGrid tg;
};