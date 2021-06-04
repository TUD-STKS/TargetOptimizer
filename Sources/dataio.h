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
		return PitchTierReader(filename);
	}

	inline TextGridReader readTextGridFile(std::string filename)
	{
		return TextGridReader(filename);
	}

	inline void saveGesturalScore(Sample onset, TargetVector pitchTargets, std::string filename)
	{
		GestureWriter gwriter(filename);
		gwriter.writeTargets(onset, pitchTargets);
	}


	inline void saveCsvFile(Sample onset, TargetVector pitchTargets, std::string filename)
	{
		CsvWriter cwriter(filename);
		cwriter.writeTargets(onset, pitchTargets);
	}

	inline void savePitchTier(TimeSignal optimalF0, std::string filename)
	{
		PitchTierWriter pwriter(filename);
		pwriter.writeF0(optimalF0);
	}

	inline void saveTOFile(std::string filename)
	{
		//TOWriter twriter(filename);
		//twriter.writeTargets(Data::getInstance().onset, Data::getInstance().pitchTargets);
	}
}

