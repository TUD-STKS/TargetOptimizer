#include <dlib/string.h>
#include <fstream>
#include <cmath>
#include "PitchTierReader.h"



PitchTierReader::PitchTierReader(const std::string& pitchTierFile)
{
	std::vector<std::string> tokens = dlib::split(pitchTierFile, ".");
	m_fileName = tokens[0];
	readFile(pitchTierFile);
}

TimeSignal PitchTierReader::getF0() const
{
	return m_f0;
}

std::string PitchTierReader::getFileName() const
{
	return m_fileName;
}

void PitchTierReader::readFile(const std::string& pitchTierFile)
{
	// create a file-reading object
	std::ifstream fin;
	fin.open(pitchTierFile.c_str()); // open data file
	if (!fin.good())
	{
		throw dlib::error("[read_data_file] PitchTier input file not found!");
	}

	// container for string values
	std::string line;
	std::vector<std::string> tokens;

	// ignore first three lines
	std::getline(fin, line);
	std::getline(fin, line);
	std::getline(fin, line);

	// following lines
	while (std::getline(fin, line))
	{
		tokens = dlib::split(line, "\t");
		if (tokens.size() != 2)
		{
			throw dlib::error("Wrong PitchTier File Format!");
		}

		double time = atof(tokens[0].c_str());
		double value = atof(tokens[1].c_str());
		if (time == 0.0 && value == 0.0)
		{
			throw dlib::error("Wrong PitchTier File Format!");
		}

		Sample s = { time,hz2st(value) };
		m_f0.push_back(s);
	}

	if (m_f0.size() == 0)
	{
		throw dlib::error("Wrong PitchTier File Format!");
	}
}

double PitchTierReader::hz2st(const double val)
{
	return 12 * (std::log(val) / std::log(2));
}
