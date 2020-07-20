#include "GestureWriter.h"
#include <fstream>
#include <iomanip>

TargetVector GestureWriter::convert2VtlFormat(const Sample& onset, const TargetVector& targets)
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
	p.tau = 10;
	p.offset = o.value;
	p.duration = o.time;
	t.insert(t.begin(), p);

	return t;
}

void GestureWriter::writeTargets(const Sample& onset, const TargetVector& targets) const
{
	// create output file and write results to it
	std::ofstream fout;
	fout.open(m_file.c_str());
	fout << std::fixed << std::setprecision(6);

	// calculate VTL format
	TargetVector t = convert2VtlFormat(onset, targets);

	// write header
	fout << "<gestural_score>" << std::endl;
	fout << "\t<gesture_sequence type=\"f0-gestures\" unit=\"st\">" << std::endl;

	// write resulting optimal targets
	for (unsigned i = 0; i < t.size(); ++i)
	{
		fout << "\t\t<gesture value=\"" << t[i].offset
			<< "\" slope=\"" << t[i].slope
			<< "\" duration_s=\"" << t[i].duration
			<< "\" time_constant_s=\"" << t[i].tau / 1000.0
			<< "\" neutral=\"0\" />" << std::endl;
	}

	// write tail
	fout << "\t</gesture_sequence>" << std::endl;
	fout << "</gestural_score>" << std::endl;
}