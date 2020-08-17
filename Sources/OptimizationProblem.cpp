#include "OptimizationProblem.h"
#include <iostream>


OptimizationProblem::OptimizationProblem(const ParameterSet& parameters, const TimeSignal& originalF0, const BoundaryVector& bounds)
	: m_parameters(parameters), m_originalF0(originalF0), m_bounds(bounds), m_modelOptimalF0(bounds, originalF0[0].value) {};

void OptimizationProblem::setOptimum(const BoundaryVector& boundaries, const TargetVector& targets)
{
	m_modelOptimalF0.setBoundaries( boundaries );
	m_modelOptimalF0.setPitchTargets(targets);
}

ParameterSet OptimizationProblem::getParameters() const
{
	return m_parameters;
}

TimeSignal OptimizationProblem::getModelF0() const
{
	double samplingfrequency = SAMPLERATE; // Hz
	double dt = 1.0 / samplingfrequency;
	return m_modelOptimalF0.calculateF0(dt);
}

TargetVector OptimizationProblem::getPitchTargets() const
{
	return m_modelOptimalF0.getPitchTargets();
}

BoundaryVector OptimizationProblem::getBoundaries() const
{
	return m_modelOptimalF0.getBoundaries();
}

void OptimizationProblem::setBoundaries( const BoundaryVector& boundaries )
{
	m_bounds = boundaries;
}

Sample OptimizationProblem::getOnset() const
{
	return m_modelOptimalF0.getOnset();
}

double OptimizationProblem::getOriginalF0_Onset() const
{
	return m_originalF0.front().time;
}

double OptimizationProblem::getOriginalF0_Offset() const
{
	return m_originalF0.back().time;
}



SampleTimes OptimizationProblem::extractTimes(const TimeSignal& f0)
{
	SampleTimes times;
	for (unsigned i = 0; i < f0.size(); ++i)
	{
		times.push_back(f0[i].time);
	}

	return times;
}

DlibVector OptimizationProblem::signal2dlibVec(const TimeSignal& f0)
{
	DlibVector values;
	values.set_size(f0.size());
	for (unsigned i = 0; i < f0.size(); ++i)
	{
		values(i) = f0[i].value;
	}

	return values;
}

double OptimizationProblem::getCorrelationCoefficient() const
{
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = m_modelOptimalF0.calculateF0(times);

	// Dlib format
	DlibVector orig = signal2dlibVec(m_originalF0);
	DlibVector model = signal2dlibVec(modelF0);

	// return correlation between filtered and original f0
	DlibVector x = orig - dlib::mean(orig);
	DlibVector y = model - dlib::mean(model);
	return (dlib::dot(x, y)) / ((std::sqrt(dlib::sum(dlib::squared(x)))) * (std::sqrt(dlib::sum(dlib::squared(y)))));

}

double OptimizationProblem::getSquareCorrelationCoefficient( const TamModelF0& tamF0 ) const
{
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = tamF0.calculateF0(times);

	// Dlib format
	DlibVector orig = signal2dlibVec(m_originalF0);
	DlibVector model = signal2dlibVec(modelF0);

	// return correlation between filtered and original f0
	DlibVector x = orig - dlib::mean(orig);
	DlibVector y = model - dlib::mean(model);
	return dlib::dot(x, y) * dlib::dot(x, y) / ( dlib::sum(dlib::squared(x)) * dlib::sum(dlib::squared(y)) );

}

double OptimizationProblem::getRootMeanSquareError() const
{
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = m_modelOptimalF0.calculateF0(times);

	// Dlib format
	DlibVector orig = signal2dlibVec(m_originalF0);
	DlibVector model = signal2dlibVec(modelF0);

	// return RMSE between filtered and original f0
	return std::sqrt(dlib::mean(dlib::squared(model - orig)));
}

double OptimizationProblem::getMeanSquareError( const TamModelF0& tamF0 ) const
{
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = tamF0.calculateF0(times);

	// Dlib format
	DlibVector orig = signal2dlibVec(m_originalF0);
	DlibVector model = signal2dlibVec(modelF0);

	// return MSE between filtered and original f0
	return dlib::mean(dlib::squared(model - orig));
}

std::tuple<double, double> OptimizationProblem::getOptStats( const BoundaryVector& boundaries, const TargetVector& targets ) const
{
	TamModelF0 tamF0(boundaries, m_originalF0[0].value);
	tamF0.setPitchTargets(targets);

	double MSE = getMeanSquareError( tamF0 );
	double SSC = getSquareCorrelationCoefficient( tamF0 );

	return std::make_tuple(MSE, SSC);
}

double OptimizationProblem::operator() (const DlibVector& arg) const
{
	// convert data
	TargetVector targets;
	BoundaryVector boundaries = m_bounds;
	//boundaries.front() = getOriginalF0_Onset();
	//boundaries.back()  = getOriginalF0_Offset();



	double modified_Bound = 0.0;
	double number_Targets = arg.size() / m_parameters.searchSpaceParameters.numberOptVar;
	//for (unsigned i = 0; i < arg.size() / m_parameters.numberOptVar; ++i)
	if ( m_parameters.searchSpaceParameters.optimizeBoundaries == true )
	{
		boundaries.back()  = getOriginalF0_Offset();
		for (unsigned i = 0; i < number_Targets; ++i)
		{
			boundaries.at(i) += arg(m_parameters.searchSpaceParameters.numberOptVar * i +3)/1000;
		}
		std::sort( boundaries.begin(), boundaries.end() );
		boundaries.back()  = getOriginalF0_Offset();
	}

	for (unsigned i = 0; i < number_Targets; ++i)
	{
		PitchTarget pt;
		pt.slope = arg(m_parameters.searchSpaceParameters.numberOptVar * i + 0);
		pt.offset = arg(m_parameters.searchSpaceParameters.numberOptVar * i + 1);
		pt.tau = arg(m_parameters.searchSpaceParameters.numberOptVar * i + 2);
		pt.duration = ( boundaries.at(i+1) - boundaries.at(i) );
		//pt.duration = m_bounds[i + 1] - m_bounds[i];// (m_bounds[i] + arg(4 * i +3)/1000);
//		if ( m_parameters.searchSpaceParameters.optimizeBoundaries == true )
//		{
//			//if ( (i >0) && (number_Targets > 1) )
//			//{
//				//boundaries.at(i) += arg(m_parameters.searchSpaceParameters.numberOptVar * i +3)/1000;
//			//}
//			//std::cout << "b: opt bound true " << m_parameters.optimizeBoundaries << std::endl;
//			//modified_Bound = m_bounds[i] + arg(m_parameters.searchSpaceParameters.numberOptVar * i +3)/1000;
//			//if ( (i==0) && (modified_Bound > m_originalF0[0].time))
//			//{
//			//	modified_Bound = m_originalF0[0].time;
//			//}
//			//boundaries.push_back( modified_Bound );
//			//boundaries.push_back( m_bounds[i] + arg(m_parameters.numberOptVar * i +3)/1000 );
//			//pt.duration = ( m_bounds[i + 1] + arg(m_parameters.searchSpaceParameters.numberOptVar * (i+1) +3)/1000 ) - boundaries[i];
//			pt.duration = ( boundaries.at(i+1) - boundaries.at(i) )
//		}
//		else{
//			//std::cout << "b: opt bound false " << m_parameters.optimizeBoundaries << std::endl;
//			pt.duration = m_bounds[i + 1] - m_bounds[i];
//		}
		targets.push_back(pt);
	}
//	if (m_parameters.searchSpaceParameters.optimizeBoundaries) //(!m_bounds.empty()
//	{
//		//modified_Bound = m_bounds.back() + arg(m_parameters.searchSpaceParameters.numberOptVar * number_Targets +3)/1000;
//		//if ( modified_Bound < m_originalF0.back().time )
//		//{
//		//	modified_Bound = m_originalF0.back().time;
//		//}
//		//boundaries.push_back( modified_Bound );
//		std::sort( boundaries.begin(), boundaries.end() );
//		//boundaries.push_back( m_bounds.back() + arg(m_parameters.numberOptVar * (arg.size() / m_parameters.numberOptVar) +3)/1000 );
//	}
//	else
//	{
//		boundaries = m_bounds;
//	}
	//std::cout << "b: " << boundaries.at(0) << " "<< boundaries.at(1) << " "<< boundaries.at(2) << " "<< boundaries.at(3) 
	//std::cout << " m: " << m_bounds.at(0) << " "<< m_bounds.at(1) << " " << m_bounds.at(2)<< " " << m_bounds.at(3) << std::endl;

	// create model f0
	//TamModelF0 tamF0(m_bounds, m_originalF0[0].value);
	TamModelF0 tamF0(boundaries, m_originalF0[0].value);
	tamF0.setPitchTargets(targets);

	return costFunction(tamF0);
}

double OptimizationProblem::costFunction(const TamModelF0& tamF0) const // TODO: pow(x,2) is much slower than x*x (depending on compiler opt), Change it
{
	// get model f0
	SampleTimes times = extractTimes(m_originalF0);
	TimeSignal modelF0 = tamF0.calculateF0(times);

	// calculate error
	double error = 0.0;
	for (unsigned i = 0; i < modelF0.size(); ++i)
	{
		error += std::pow((m_originalF0[i].value - modelF0[i].value), 2.0);
	}
	//std::cout << "error " << error << std::endl;

	// calculate penalty term
	double penalty = 0.0;
	TargetVector targets = tamF0.getPitchTargets();
	for (unsigned i = 0; i < targets.size(); ++i)

	{
		penalty += (m_parameters.regularizationParameters.weightSlope * std::pow(targets[i].slope - m_parameters.searchSpaceParameters.meanSlope, 2.0));
		penalty += (m_parameters.regularizationParameters.weightOffset * std::pow(targets[i].offset - m_parameters.searchSpaceParameters.meanOffset, 2.0));
		penalty += (m_parameters.regularizationParameters.weightTau * std::pow(targets[i].tau - m_parameters.searchSpaceParameters.meanTau, 2.0));
	}

	return error + m_parameters.regularizationParameters.lambda * penalty;
}