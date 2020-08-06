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
	Tier(std::string str);	

	bool append(TierType element);
	int size();
	TierType& getElement(int i);
	bool setElementStart(int elementIndex, double newStart);
	bool setElementEnd(int elementIndex, double newEnd);
	double getStartingTime();
	double getEndingTime();

	void setConsecutive(bool);

public:
	std::string name;

	auto operator[](int index) { return getElement(index); };

private:
	std::vector<TierType> elements;
	bool mustBeConsecutive;
};

template<class TierType>
inline Tier<TierType>::Tier(std::string str)
{
	auto type = (std::string)typeid(this).name(); 
	if (type == "class Tier<class Interval> *") {
		mustBeConsecutive = true;
	}
	else if (type == "class Tier<class Point> *") {
		mustBeConsecutive = false;
	}
	name = str;
}

template<class TierType>
inline bool Tier<TierType>::append(TierType element)
{
	if (this->mustBeConsecutive) {
		if (this->elements.size() != 0)	{
			double newElementStart = element.getStart();
			if (newElementStart == this->elements.back().getEnd()) {
				this->elements.push_back(element);
				std::cout << "Interval has successfully been added!" << std::endl;
				return true;
			}
			else {
				std::cout << "Intervals have to be consecutive! Starting time must be at " << this->elements.back().getEnd() << "!" << std::endl;
				return false;
			}
		}		
		else {
			this->elements.push_back(element);
			std::cout << "Interval has successfully been added!" << std::endl;
			return true;
		}
	}
	else {
		std::string type = (std::string)typeid(element).name();
		if (type == "class Interval") {
			if (this->elements.size() != 0) {
				double newElementStart = element.getStart();
				if (newElementStart >= this->elements.back().getEnd()) {
					this->elements.push_back(element);
					std::cout << "Interval has successfully been added!" << std::endl;
					return true;
				}
				else {
					std::cout << "Starting time of the new interval has to be later than the ending time of previous interval (" << this->elements.back().getEnd() << ")!" << std::endl;
					return false;
				}
			}
			else {
				this->elements.push_back(element);
				std::cout << "Interval has successfully been added!" << std::endl;
				return true;
			}
		}
		else if (type == "class Point") {
			this->elements.push_back(element);
			std::cout << "Point has successfully been added!" << std::endl;
			return true;
		}		
	}
	return false;
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
	if ((elementIndex >= 0 && elementIndex < (signed int)elements.size()) && newStart >= 0) {
		if (newStart < elements[elementIndex].tmax) {
			if (elementIndex != 0) {
				if (newStart > elements[elementIndex - 1].tmin) {
					elements[elementIndex - 1].tmax = newStart;
				}
				else {
					std::cout << "Starting time must be later than the starting time of previous interval (" << elements[elementIndex - 1].tmin << ") !" << std::endl;
					return false;
				}
			}
			elements[elementIndex].tmin = newStart;
			std::cout << "Starting time has successfully been changed!" << std::endl;
			return true;
		}
		else
		{
			std::cout << "Starting time must be earlier than the ending time of selected interval (" << elements[elementIndex].tmax << ") !" << std::endl;
			return false;
		}		
	}
	else {
		std::cout << "Please use a valid element index (0 < index < numIntervals/numPoints) and a valid starting time (> 0)!" << std::endl;
		return false;
	}
}

template<class TierType>
inline bool Tier<TierType>::setElementEnd(int elementIndex, double newEnd)
{
	if ((elementIndex >= 0 && elementIndex < (signed int)elements.size()) && newEnd >= 0) {
		if (newEnd > elements[elementIndex].tmin) {
			if (elementIndex != elements.size() - 1) {
				if (newEnd < elements[elementIndex + 1].tmax) {
					elements[elementIndex + 1].tmin = newEnd;
				}
				else {
					std::cout << "Ending time must be earlier than the ending time of following interval (" << elements[elementIndex + 1].tmax << ") !" << std::endl;
					return false;
				}
					
			}
			elements[elementIndex].tmax = newEnd;
			std::cout << "Ending time has successfully been changed!" << std::endl;
			return true;
		}
		else {
			std::cout << "Ending time must be later than the starting time of selected interval (" << elements[elementIndex].tmin << ") !" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "Please use a valid element index (0 < index < numIntervals) and a valid ending time (> 0)!" << std::endl;
		return false;
	}
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

template<class TierType>
inline void Tier<TierType>::setConsecutive(bool flag)
{
	this->mustBeConsecutive = flag;
}