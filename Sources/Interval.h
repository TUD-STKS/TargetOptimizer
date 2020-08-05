#pragma once

#include <string>
#include "Tier.h"

class Interval {
public:
	// constructors
	Interval() = delete;
	Interval(double tmin, double tmax, std::string text);

	double getStart();
	double getEnd();
	void changeOffset(double offset);

public:
	std::string text;

private:
	//TODO: Allow Tier access to these functions (friend?)
	template <class TierType>
	friend bool Tier<TierType>::setElementStart(int, double);
	template <class TierType>
	friend bool Tier<TierType>::setElementEnd(int, double);

private:
	double tmin;
	double tmax;
};

