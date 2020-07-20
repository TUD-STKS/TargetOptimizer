#pragma once
#include <string>
#include "Data.h"

class TextGridReader {
public:
	// constructors
	TextGridReader(const std::string& textGridFile);

	// public member functions
	BoundaryVector getBounds() const;

private:
	// private member functions
	void readFile(const std::string& textGridFile);
	static bool checkDigits(const std::string& s);

	// data members
	BoundaryVector m_bounds;
};