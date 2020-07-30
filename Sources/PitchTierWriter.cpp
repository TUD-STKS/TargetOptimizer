#include <iomanip>
#include <fstream>
#include <cmath>
#include "PitchTierWriter.h"


void PitchTierWriter::writeF0(const TimeSignal& f0) const
{
	// create output file and write results to it
	unsigned K = f0.size();
	std::ofstream fout;
	fout.open(m_file.c_str());
	fout << std::fixed << std::setprecision(6);

	// write header
	fout << "\"ooTextFile\"" << std::endl;
	fout << "\"PitchTier\"" << std::endl;
	fout << 0 << " " << f0[K - 1].time + 0.1 << " " << K << std::endl;

	// write optimal f0
	for (unsigned i = 0; i < f0.size(); ++i)
	{
		fout << f0[i].time << "\t" << f0[i].value << std::endl;
	}
}
