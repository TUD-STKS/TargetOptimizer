#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <typeinfo>

template<class TierType> class Tier;

template<class TierType> class Tier {

public:
	Tier() = delete;
	// TODO: What happens to copy constructor? Delete if necessary
	// Copy constructor works!
	Tier(std::string str);	

	void append(TierType element);
	int size();
	TierType& getElement(int i);
	bool setElementStart(int elementIndex, double newStart);
	bool setElementEnd(int elementIndex, double newEnd);
	double getStartingTime();
	double getEndingTime();

public:
	std::string name;

	auto operator[](int index) { return getElement(index); };

private:
	std::vector<TierType> elements;
};

template<class TierType>
inline Tier<TierType>::Tier(std::string str)
{
	name = str;
}

template<class TierType>
inline void Tier<TierType>::append(TierType element)
{
	this->elements.push_back(element);
}

template<class TierType>
inline int Tier<TierType>::size()
{
	return this->elements.size();
}

template<class TierType>
inline TierType& Tier<TierType>::getElement(int i)
{
	return this->elements.at(i);
}

template<class TierType>
inline bool Tier<TierType>::setElementStart(int elementIndex, double newStart)
{
	elements[elementIndex].tmin = newStart;
	return false;
}

template<class TierType>
inline bool Tier<TierType>::setElementEnd(int elementIndex, double newEnd)
{
	//TODO: Move end of selected element and start of next element
	return false;
}

template<class TierType>
inline double Tier<TierType>::getStartingTime()
{
	return std::min_element(this->elements.begin(), this->elements.end(), [](auto& a, auto& b) {return a.getStart() < b.getStart(); })->getStart();
}

template<class TierType>
inline double Tier<TierType>::getEndingTime()
{
	return std::max_element(this->elements.begin(), this->elements.end(), [](auto& a, auto& b) {return a.getEnd() < b.getEnd(); })->getEnd();
}
