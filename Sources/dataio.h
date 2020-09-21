#pragma once
#include "CsvWriter.h"
#include "GestureWriter.h"
#include "PitchTierWriter.h"
#include "PitchTierReader.h"
#include "TextGridReader.h"

namespace DataIO
{
	inline PitchTierReader readPitchTierFile(std::string filename)
	{
		return PitchTierReader(std::string(filename));
	}

	inline TextGridReader readTextGridFile(std::string filename)
	{
		return TextGridReader(std::string(filename));
	}

	inline void saveGesturalScore(std::string filename)
	{
		GestureWriter gwriter(filename);
		gwriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
	}


	inline void saveCsvFile(std::string filename)
	{
		CsvWriter cwriter(filename);
		cwriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
	}

	inline void savePitchTier(std::string filename)
	{
		PitchTierWriter pwriter(filename);
		pwriter.writeF0(Data::getInstance().optimalF0);
	}

	inline void saveTOFile(std::string filename)
	{
		//TOWriter twriter(filename);
		//twriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
	}
}

