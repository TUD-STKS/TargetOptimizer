#pragma once
#include <string>
#include "Data.h"

class PitchTierWriter {
public:
	// constructors
	PitchTierWriter(const std::string& pitchTierFile) : m_file(pitchTierFile) {};

	// public member functions
	void writeF0(const TimeSignal& f0) const;

private:
	// data members
	std::string m_file;
};