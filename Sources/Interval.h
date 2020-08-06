#pragma once
#include <string>
#include "Tier.h"

class Interval {
public:
	// constructors
	Interval() = delete;
	Interval(double tmin, double tmax, std::string text, bool isConsecutive = true);

	double getStart();
	double getEnd();
	void changeOffset(double offset);

	bool getConsecutiveness();

public:
	std::string text;

private:
	template <class TierType>
	friend bool Tier<TierType>::setElementStart(int, double);
	template <class TierType>
	friend bool Tier<TierType>::setElementEnd(int, double);

private:
	double tmin;
	double tmax;

	bool isConsecutive;
};

