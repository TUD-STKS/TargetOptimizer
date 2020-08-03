#include <string>

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
	bool setStart(double newStart);
	bool setEnd(double newEnd);

private:
	double tmin;
	double tmax;
	
};