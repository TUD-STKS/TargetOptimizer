#pragma once
#include <string>
#include "Data.h"

class PitchTierReader {
public:
	// constructors
	PitchTierReader(const std::string& pitchTierFile);

	// public member functions
	TimeSignal getF0() const;
	std::string getFileName() const;

private:
	// private member functions
	void readFile(const std::string& pitchTierFile);
	inline double hz2st(const double val);

	// data members
	TimeSignal m_f0;
	std::string m_fileName;
};