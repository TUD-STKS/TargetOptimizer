#pragma once
#include "Data.h"

class CsvWriter {
public:
	// constructors
	CsvWriter(const std::string& csvFile) : m_file(csvFile) {};

	// public member functions
	void writeTargets(const Sample& onset, const TargetVector& targets) const;

private:
	// private member functions
	static TargetVector convert2VtlFormat(const Sample& onset, const TargetVector& targets);

	// data members
	std::string m_file;
};