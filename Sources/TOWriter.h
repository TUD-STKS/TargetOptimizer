#pragma once
#include "Data.h"

class TOWriter {
public:
	// constructors
	TOWriter( const std::string& TOFile ) : m_file( TOFile ) {};

	// public member functions
	void writeLOG(const Sample& onset, const TargetVector& optTargets, const BoundaryVector& optBoundaries) const;
	//void writeLOG(const Sample& onset, const TargetVector& targets) const;

private:
	// data members
	std::string m_file;
};