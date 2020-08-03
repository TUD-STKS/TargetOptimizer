#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include "Interval.h"
#include "Point.h"


template<class TierType> class Tier;

template<class TierType> class Tier {

public:
	Tier() = delete;
	// What happens to copy constructor? Delete if necessary
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
	//TODO: Move end of previous element and start of selected element
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
	return std::min(elements.begin()->getStart(), elements.end()->getStart());
}

template<class TierType>
inline double Tier<TierType>::getEndingTime()
{
	return std::max(elements.begin()->getEnd(), elements.end()->getEnd());
}
