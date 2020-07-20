#include <fstream>
#include <iomanip>
#include "CsvWriter.h"



TargetVector CsvWriter::convert2VtlFormat(const Sample& onset, const TargetVector& targets)
{
	// local copy
	TargetVector t(targets);
	Sample o(onset);

	// check onset time
	if (o.time < 0.01)
	{
		t[0].duration -= (0.01 - o.time);
		o.time = 0.01;
	}

	// add "silent" target
	PitchTarget p;
	p.slope = 0.0;
	p.tau = 0.01;
	p.offset = o.value;
	p.duration = o.time;
	t.insert(t.begin(), p);

	return t;
}

void CsvWriter::writeTargets(const Sample& onset, const TargetVector& targets) const
{
	// create output file and write results to it
	std::ofstream fout;
	fout.open(m_file.c_str());
	fout << std::fixed << std::setprecision(6);

	// calculate VTL format
	TargetVector t = convert2VtlFormat(onset, targets);

	// write header
	fout << "target-No.,slope[st/s],offset[st],tau[s],duration[s]" << std::endl;

	// write optimal targets
	for (unsigned i = 0; i < t.size(); ++i)
	{
		fout << i << "," << t[i].slope << "," << t[i].offset << "," << t[i].tau << "," << t[i].duration << std::endl;
	}
}