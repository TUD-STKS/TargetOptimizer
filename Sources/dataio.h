#ifndef DATAIO_H_
#define DATAIO_H_

#include <string>
#include <dlib/gui_widgets.h>
#include "model.h"

using namespace dlib;

class TextGridReader {
public:
	// constructors
	TextGridReader (const std::string &textGridFile);

	// public member functions
	BoundVector getBounds() const;

private:
	// private member functions
	void readFile(const std::string &textGridFile);
	static bool checkDigits(const std::string &s);

	// data members
	BoundVector m_bounds;
};

class PitchTierReader {
public:
	// constructors
	PitchTierReader (const std::string &pitchTierFile);

	// public member functions
	TimeSignal getF0() const;
	std::string getFileName() const;

private:
	// private member functions
	void readFile(const std::string &pitchTierFile);
	inline double hz2st (const double val);

	// data members
	TimeSignal m_f0;
	std::string m_fileName;
};

class PitchTierWriter {
public:
	// constructors
	PitchTierWriter (const std::string &pitchTierFile) : m_file(pitchTierFile) {};

	// public member functions
	void writeF0(const TimeSignal &f0) const;

private:
	// data members
	std::string m_file;
};

class GestureWriter {
public:
	// constructors
	GestureWriter (const std::string &gestureFile) : m_file(gestureFile) {};

	// public member functions
	void writeTargets(const Sample &onset, const TargetVector &targets) const;

private:
	// private member functions
	static TargetVector convert2VtlFormat(const Sample& onset, const TargetVector &targets);

	// data members
	std::string m_file;
};

class CsvWriter {
public:
	// constructors
	CsvWriter (const std::string &csvFile) : m_file(csvFile) {};

	// public member functions
	void writeTargets(const Sample &onset, const TargetVector &targets) const;

private:
	// priate member functions
	static TargetVector convert2VtlFormat(const Sample& onset, const TargetVector &targets);

	// data members
	std::string m_file;
};

struct SignalStat
{
	double minTime;
	double maxTime;
	double minValue;
	double maxValue;
};

class PlotRegion : public zoomable_region
{
public:
	PlotRegion (drawable_window& w);
	~PlotRegion ();

	void setBounds(const BoundVector &bounds);
	void setOrigF0(const TimeSignal &f0);
	void setOptimalF0(const TimeSignal &f0);
	void setTargets(const TargetVector &targets);

private:
    void draw (const canvas& c) const;
    SignalStat analyzeSignal(const TimeSignal &f0) const;

    TimeSignal m_optF0;
    TimeSignal m_origF0;
    BoundVector m_bounds;
    TargetVector m_targets;
};

class MainWindow : public drawable_window
{
public:
	MainWindow();
    ~MainWindow();

private:

    // Private helper methods
    void noOptimizationPerformed();
    ParameterSet readParameters();

    // Event handlers
    void onButtonClear ();
    void onButtonTextGridOpen ();
    void onButtonPitchTierOpen ();
    void onButtonOptimize ();
    void openTextGrid ( const std::string& fileName);
    void openPitchTier ( const std::string& fileName);
    void onReadyForOptimize ();
    void optimize ();
    void onSaveFileGesture (const std::string& fileName);
    void onButtonSaveAsGesture();
    void onSaveFileCsv (const std::string& fileName);
    void onMenuSaveAsCsv();
    void onSaveFilePitchTier (const std::string& fileName);
    void onMenuSaveAsPitchTier();
    void unblockMainWindow();
    void onMenuFileQuit();
    void onMenuFileHelp();
    void onMenuFileAbout();
    void on_window_resized();

    // GUI widgets
    PlotRegion graph;
    button btnLoadTextGrid;
    button btnLoadPitchTier;
    button btnOptimize;
    button btnStoreGesture;
    button btnStoreCsv;
    button btnClear;
    menu_bar mbar;
    named_rectangle recTargets;
    named_rectangle recOptions;
    named_rectangle recActions;
    text_grid targetGrid;
    widget_group searchSpaceGroup;
    widget_group penaltyGroup;
    tabbed_display tabs;
    label lbSlope, lbOffset, lbTau, lbLambda, lbWeightm, lbWeightb, lbWeighttau;
    text_field tfSlope, tfOffset, tfTau, tfLambda, tfWeightm, tfWeightb, tfWeighttau;

    // data members
    TargetVector m_optTarget;
    Sample m_optOnset;
    TimeSignal m_optF0;
    TimeSignal m_origF0;
    BoundVector m_bounds;
};

#endif /* DATAIO_H_ */
