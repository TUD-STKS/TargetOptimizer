#pragma once
#include <string>
#include "Data.h"

class GestureWriter {
public:
	// constructors
	GestureWriter(const std::string& gestureFile) : m_file(gestureFile) {};

	// public member functions
	void writeTargets(const Sample& onset, const TargetVector& targets) const;

private:
	// private member functions
	static TargetVector convert2VtlFormat(const Sample& onset, const TargetVector& targets);

	// data members
	std::string m_file;
};