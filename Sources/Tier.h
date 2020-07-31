#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include "Interval.h"
#include "Point.h"


template<class TierType> class Tier;

typedef Tier<Interval>	IntervalTier;
typedef Tier<Point>		PointTier;

template<class TierType> class Tier {

public:
	Tier(double xmin, double xmax, std::string str);	

	void append(TierType element);
	int getNumberOfElements();
	TierType& getElement(int i);
	std::string getName();
	void renameTier(std::string newName);
	void renameElement(int i, std::string newName);
	double getStartingTime();
	double getEndingTime();

	auto operator[](int index) { return getElement(index); };

	int numberOfElements;

private:
	std::vector<TierType> elements;
	double tmin; 
	double tmax;
	std::string name;	
};

template<class TierType>
inline Tier<TierType>::Tier(double xmin, double xmax, std::string str)
{
	tmin = xmin;
	tmax = xmax;
	name = str;

	numberOfElements = 0;
}

template<class TierType>
inline void Tier<TierType>::append(TierType element)
{
	this->elements.push_back(element);
	this->tmax = std::max(element.getEnd(), this->tmax);
	this->tmin = std::min(element.getStart(), this->tmin);
	this->numberOfElements = getNumberOfElements();
}

template<class TierType>
inline int Tier<TierType>::getNumberOfElements()
{
	return this->elements.size();
}

template<class TierType>
inline TierType& Tier<TierType>::getElement(int i)
{
	try {
		return this->elements.at(i);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of range error: " << oor.what() << '\n';
	}	
}

template<class TierType>
inline std::string Tier<TierType>::getName()
{
	return this->name;
}

template<class TierType>
inline void Tier<TierType>::renameTier(std::string newName)
{
	this->name = newName;
}

template<class TierType>
inline void Tier<TierType>::renameElement(int i, std::string newName)
{
	this->getElement(i).changeText(newName);
}

template<class TierType>
inline double Tier<TierType>::getStartingTime()
{
	return this->tmin;
}

template<class TierType>
inline double Tier<TierType>::getEndingTime()
{
	return this->tmax;
}
