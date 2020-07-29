class Point
{
public:
	Point(double time, double value);

	double time();
	double mark();
	void changeOffset(double offset);

private:
	double x;
	double y;

};

