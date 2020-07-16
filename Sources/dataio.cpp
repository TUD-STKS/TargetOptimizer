#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <dlib/string.h>
#include <dlib/misc_api.h>
#include "dataio.h"


TextGridReader::TextGridReader (const std::string &textGridFile)
{
	readFile(textGridFile);
}

BoundVector TextGridReader::getBounds() const
{
	return m_bounds;
}

void TextGridReader::readFile(const std::string &textGridFile)
{
	// create a file-reading object
	std::ifstream fin;
	fin.open(textGridFile.c_str()); // open data file
	if (!fin.good())
	{
		throw dlib::error("[read_data_file] TextGrid input file not found!");
	}

	// container for string values
	std::string line, line_1, line_2;

	// process lines
	while(std::getline(fin, line))
	{
		if(line == "\"1\"")
		{
			m_bounds.push_back(atof(line_2.c_str()));
			m_bounds.push_back(atof(line_1.c_str()));
		}
		else if(!line.substr(0,1).compare("\""))
		{
			line = dlib::trim(line,"\"");
			if (checkDigits(line) && !line.empty())
			{
				m_bounds.push_back(atof(line_1.c_str()));
			}
		}

		// store temporary
		line_2 = line_1;
		line_1 = line;
	}

	if (m_bounds.size() == 0)
	{
		throw dlib::error("Wrong TextGrid File Format!");
	}
}

bool TextGridReader::checkDigits(const std::string &s)
{
  return s.find_first_not_of("0123456789") == std::string::npos;
}

PitchTierReader::PitchTierReader (const std::string &pitchTierFile)
{
	std::vector<std::string> tokens = dlib::split(pitchTierFile, ".");
	m_fileName = tokens[0];
	readFile(pitchTierFile);
}

TimeSignal PitchTierReader::getF0() const
{
	return m_f0;
}

std::string PitchTierReader::getFileName() const
{
	return m_fileName;
}

void PitchTierReader::readFile(const std::string &pitchTierFile)
{
	// create a file-reading object
	std::ifstream fin;
	fin.open(pitchTierFile.c_str()); // open data file
	if (!fin.good())
	{
		throw dlib::error("[read_data_file] PitchTier input file not found!");
	}

	// container for string values
	std::string line;
	std::vector<std::string> tokens;

	// ignore first three lines
	std::getline(fin, line);
	std::getline(fin, line);
	std::getline(fin, line);

	// following lines
	while(std::getline(fin, line))
	{
		tokens = dlib::split(line, "\t");
		if (tokens.size() != 2)
		{
			throw dlib::error("Wrong PitchTier File Format!");
		}

		double time = atof(tokens[0].c_str());
		double value = atof(tokens[1].c_str());
		if (time == 0.0 && value == 0.0)
		{
			throw dlib::error("Wrong PitchTier File Format!");
		}

		Sample s = {time,hz2st(value)};
		m_f0.push_back(s);
	}

	if (m_f0.size() == 0)
	{
		throw dlib::error("Wrong PitchTier File Format!");
	}
}

double PitchTierReader::hz2st (const double val)
{
	return 12*(std::log(val)/std::log(2));
}

void PitchTierWriter::writeF0(const TimeSignal &f0) const
{
	// create output file and write results to it
	unsigned K = f0.size();
	std::ofstream fout;
	fout.open(m_file.c_str());
	fout << std::fixed << std::setprecision(6);

	// write header
	fout << "\"ooTextFile\"" << std::endl;
	fout << "\"PitchTier\"" << std::endl;
	fout << 0 << " " << f0[K-1].time+0.1 << " " << K << std::endl;

	// write optimal f0
	for (unsigned i=0; i<f0.size(); ++i)
	{
		fout << f0[i].time << "\t" << f0[i].value << std::endl;
	}
}

TargetVector GestureWriter::convert2VtlFormat(const Sample& onset, const TargetVector &targets)
{
	// local copy
	TargetVector t (targets);
	Sample o (onset);

	// check onset time
	if (o.time < 0.01)
	{
		t[0].duration -= (0.01 - o.time);
		o.time = 0.01;
	}

	// add "silent" target
	PitchTarget p;
	p.slope = 0.0;
	p.tau = 10;
	p.offset = o.value;
	p.duration = o.time;
	t.insert(t.begin(),p);

	return t;
}

void GestureWriter::writeTargets(const Sample &onset, const TargetVector &targets) const
{
	// create output file and write results to it
	std::ofstream fout;
	fout.open(m_file.c_str());
	fout << std::fixed << std::setprecision(6);

	// calculate VTL format
	TargetVector t = convert2VtlFormat(onset, targets);

	// write header
	fout << "<gestural_score>" << std::endl;
	fout << "\t<gesture_sequence type=\"f0-gestures\" unit=\"st\">" << std::endl;

	// write resulting optimal targets
	for (unsigned i=0; i<t.size(); ++i)
	{
		fout << "\t\t<gesture value=\"" 	<< t[i].offset
			 << "\" slope=\"" 			<< t[i].slope
			 << "\" duration_s=\"" 		<< t[i].duration
			 << "\" time_constant_s=\"" << t[i].tau/1000.0
			 << "\" neutral=\"0\" />"		<< std::endl;
	}

	// write tail
	fout << "\t</gesture_sequence>" << std::endl;
	fout << "</gestural_score>" << std::endl;
}

TargetVector CsvWriter::convert2VtlFormat(const Sample& onset, const TargetVector &targets)
{
	// local copy
	TargetVector t (targets);
	Sample o (onset);

	// check onset time
	if (o.time < 0.01)
	{
		t[0].duration -= (0.01 - o.time);
		o.time = 0.01;
	}

	// add "silent" target
	PitchTarget p;
	p.slope = 0.0;
	p.tau = 0.01;
	p.offset = o.value;
	p.duration = o.time;
	t.insert(t.begin(),p);

	return t;
}

void CsvWriter::writeTargets(const Sample &onset, const TargetVector &targets) const
{
	// create output file and write results to it
	std::ofstream fout;
	fout.open(m_file.c_str());
	fout << std::fixed << std::setprecision(6);

	// calculate VTL format
	TargetVector t = convert2VtlFormat(onset, targets);

	// write header
	fout << "target-No.,slope[st/s],offset[st],tau[s],duration[s]" << std::endl;

	// write optimal targets
	for (unsigned i=0; i<t.size(); ++i)
	{
		fout << i << "," << t[i].slope << "," << t[i].offset << "," << t[i].tau << "," << t[i].duration << std::endl;
	}
}

PlotRegion::PlotRegion (drawable_window& w) : zoomable_region(w,MOUSE_CLICK | MOUSE_WHEEL | KEYBOARD_EVENTS)
{
	enable_events();
}

PlotRegion::~PlotRegion ()
{
	disable_events();
	parent.invalidate_rectangle(rect);
}

void PlotRegion::setBounds(const BoundVector &bounds)
{
	m_bounds = bounds;
	parent.invalidate_rectangle(rect);
};

void PlotRegion::setOrigF0(const TimeSignal &f0)
{
	m_origF0 = f0;
	parent.invalidate_rectangle(rect);
}

void PlotRegion::setOptimalF0(const TimeSignal &f0)
{
	m_optF0 = f0;
	parent.invalidate_rectangle(rect);
}

void PlotRegion::setTargets(const TargetVector &targets)
{
	m_targets = targets;
	parent.invalidate_rectangle(rect);
}

void PlotRegion::draw (const canvas& c) const
{
	zoomable_region::draw(c);

	rectangle area = c.intersect(display_rect());
	if (area.is_empty() == true)
		return;

	if (enabled)
		fill_rect(c,display_rect(),255);
	else
		fill_rect(c,display_rect(),128);

	SignalStat scaler = {0,0,0,0};
	// draw bounds
	if (!m_bounds.empty())
	{
		scaler.minTime = m_bounds[0]-0.025;
		scaler.maxTime = m_bounds[m_bounds.size()-1]+0.05;
		for (unsigned i = 0; i < m_bounds.size(); ++i)
		{
			double position = ((m_bounds[i]-scaler.minTime)/(scaler.maxTime-scaler.minTime))*width();
			point p(graph_to_gui_space(point(position,0)));
			point p2(graph_to_gui_space(point(position,height())));
			draw_line(c,p2,p ,rgb_pixel(0,0,0), area);
		}
	}

	// draw original f0
	if (!m_origF0.empty())
	{
		SignalStat stat = analyzeSignal(m_origF0);
		scaler.maxValue = stat.maxValue+5;
		scaler.minValue = stat.minValue-5;
		if (scaler.maxTime == 0.0)
		{
			scaler.maxTime = stat.maxTime+0.05;
			scaler.minTime = stat.minTime-0.025;
		}
		for (unsigned i=0; i<m_origF0.size(); ++i)
		{
			double x = ((m_origF0[i].time-scaler.minTime)/(scaler.maxTime-scaler.minTime))*width();
			double y = height()-((m_origF0[i].value-scaler.minValue)/(scaler.maxValue-scaler.minValue))*height();
			draw_solid_circle(c, graph_to_gui_space(point(x,y)), 2, rgb_pixel(0,0,255));
		}
	}

	// draw optimal f0
	if(!m_optF0.empty())
	{
		for (unsigned i=0; i<m_optF0.size(); ++i)
		{
			double x = ((m_optF0[i].time-scaler.minTime)/(scaler.maxTime-scaler.minTime))*width();
			double y = height()-((m_optF0[i].value-scaler.minValue)/(scaler.maxValue-scaler.minValue))*height();

			if (y>=0 && y<=height()-2)
			{
				draw_solid_circle(c, graph_to_gui_space(point(x,y)), 1.5, rgb_pixel(100,200,100));
			}
		}
	}

	// draw optimal targets
	if(!m_targets.empty())
	{
		double begin = m_optF0[0].time;
		double end = begin;
		for (unsigned i=0; i<m_targets.size(); ++i)
		{
			begin = end;
			end = begin + m_targets[i].duration;
			double x1 = ((begin-scaler.minTime)/(scaler.maxTime-scaler.minTime))*width();
			double y1 = height()-((m_targets[i].offset-scaler.minValue)/(scaler.maxValue-scaler.minValue))*height();
			double x2 = ((end-scaler.minTime)/(scaler.maxTime-scaler.minTime))*width();
			double y2 = height()-(((m_targets[i].offset + m_targets[i].slope*m_targets[i].duration)-scaler.minValue)/(scaler.maxValue-scaler.minValue))*height();
			draw_line(c,graph_to_gui_space(point(x1,y1)), graph_to_gui_space(point(x2,y2)) ,rgb_pixel(200,100,100), area);
		}
	}
}

SignalStat PlotRegion::analyzeSignal(const TimeSignal &f0) const
{
	std::vector<double> times, values;
	for (unsigned i=0; i<f0.size(); ++i)
	{
		times.push_back(f0[i].time);
		values.push_back(f0[i].value);
	}

	SignalStat result;
	result.minTime = *std::min_element(times.begin(), times.end());
	result.maxTime = *std::max_element(times.begin(), times.end());
	result.minValue = *std::min_element(values.begin(), values.end());
	result.maxValue = *std::max_element(values.begin(), values.end());

	return result;
}

MainWindow::MainWindow() :
	graph(*this),
	btnLoadTextGrid(*this),
	btnLoadPitchTier(*this),
	btnOptimize(*this),
	btnStoreGesture(*this),
	btnStoreCsv(*this),
	btnClear(*this),
    mbar(*this),
	recTargets(*this),
	recOptions(*this),
	recActions(*this),
	targetGrid(*this),
	searchSpaceGroup(*this),
	penaltyGroup(*this),
	tabs(*this),
    lbSlope(*this), lbOffset(*this), lbTau(*this), lbLambda(*this), lbWeightm(*this), lbWeightb(*this), lbWeighttau(*this),
    tfSlope(*this), tfOffset(*this), tfTau(*this), tfLambda(*this), tfWeightm(*this), tfWeightb(*this), tfWeighttau(*this)
{
    set_title("Target Optimizer");

    // position the widget that is responsible for drawing the directed graph, the graph_drawer,
    // just below the mbar (menu bar) widget.
    graph.set_pos(15,mbar.bottom()+5);
    graph.set_min_zoom_scale(1.0);
    set_size(750,520);

    // register the event handlers with their respective widgets
    btnLoadTextGrid.set_click_handler(*this, &MainWindow::onButtonTextGridOpen);
    btnLoadPitchTier.set_click_handler(*this, &MainWindow::onButtonPitchTierOpen);
    btnOptimize.set_click_handler(*this, &MainWindow::onButtonOptimize);
    btnClear.set_click_handler(*this, &MainWindow::onButtonClear);
    btnStoreGesture.set_click_handler(*this, &MainWindow::onButtonSaveAsGesture);
    btnStoreCsv.set_click_handler(*this, &MainWindow::onMenuSaveAsCsv);

    // now set the text of some of our buttons and labels
    btnLoadTextGrid.set_name("Load TextGrid");
    btnLoadPitchTier.set_name("Load PitchTier");
    btnOptimize.set_name("Optimize");
    btnClear.set_name("Clear");
    btnStoreGesture.set_name("Save as Gesture");
    btnStoreCsv.set_name("Save as CSV");
    recTargets.set_name("Targets");
    recOptions.set_name("Options");
    recActions.set_name("Actions");

    // Now setup the tabbed display.  It will have two tabs, one for the search space
    // and for regularization
    tabs.set_number_of_tabs(2);
    tabs.set_tab_name(0,"Search Space");
    tabs.set_tab_name(1,"Regularization");
    searchSpaceGroup.add(lbSlope,0,12);
    searchSpaceGroup.add(lbOffset,0,42);
    searchSpaceGroup.add(lbTau,0,72);
    searchSpaceGroup.add(tfSlope,130,10);
    searchSpaceGroup.add(tfOffset,130,40);
    searchSpaceGroup.add(tfTau,130,70);
    penaltyGroup.add(lbLambda,0,12);
    penaltyGroup.add(lbWeightm,0,42);
    penaltyGroup.add(lbWeightb,0,72);
    penaltyGroup.add(lbWeighttau,0,102);
    penaltyGroup.add(tfLambda,100,10);
    penaltyGroup.add(tfWeightm,100,40);
    penaltyGroup.add(tfWeightb,100,70);
    penaltyGroup.add(tfWeighttau,100,100);
    tabs.set_tab_group(0,searchSpaceGroup);
    tabs.set_tab_group(1,penaltyGroup);

    // Now setup the menu bar.  We will have two menus.  A File and Help menu.
    mbar.set_number_of_menus(2);
    mbar.set_menu_name(0,"File",'F');
    mbar.set_menu_name(1,"Help",'H');

    // add the entries to the File menu.
    mbar.menu(0).add_menu_item(menu_item_text("Open TextGrid", *this, &MainWindow::onButtonTextGridOpen, 'T'));
    mbar.menu(0).add_menu_item(menu_item_text("Open PitchTier", *this, &MainWindow::onButtonPitchTierOpen, 'P'));
    mbar.menu(0).add_menu_item(menu_item_separator());
    mbar.menu(0).add_menu_item(menu_item_text("Optimize", *this, &MainWindow::onButtonOptimize, 'O'));
    mbar.menu(0).add_menu_item(menu_item_separator());
    mbar.menu(0).add_menu_item(menu_item_text("Save As Gesture",*this, &MainWindow::onButtonSaveAsGesture, 'G'));
    mbar.menu(0).add_menu_item(menu_item_text("Save As CSV",*this, &MainWindow::onMenuSaveAsCsv, 'C'));
    mbar.menu(0).add_menu_item(menu_item_text("Save As PitchTier",*this, &MainWindow::onMenuSaveAsPitchTier, 'i'));
    mbar.menu(0).add_menu_item(menu_item_separator());
    mbar.menu(0).add_menu_item(menu_item_text("Clear",   *this, &MainWindow::onButtonClear,    'C'));
    mbar.menu(0).add_menu_item(menu_item_text("Quit",   *this, &MainWindow::onMenuFileQuit,    'Q'));

    // Add the entries to the Help menu.
    mbar.menu(1).add_menu_item(menu_item_text("Help",   *this, &MainWindow::onMenuFileHelp,'H'));
    mbar.menu(1).add_menu_item(menu_item_text("About",  *this, &MainWindow::onMenuFileAbout,'A'));

    // call our helper functions and window resize event to get the widgets
    // to all arrange themselves correctly in our window.
    noOptimizationPerformed();
    on_window_resized();
}

MainWindow::~MainWindow()
{
    close_window();
}

void MainWindow::noOptimizationPerformed ()
{
    btnOptimize.disable();
    btnStoreGesture.disable();
    targetGrid.disable();
    btnStoreCsv.disable();

    lbSlope.set_text("slope: 0.0 [st/s]     +/-");
    lbOffset.set_text("offset: f0-mean [st] +/-");
    lbTau.set_text("tau:    15.0 [ms]     +/-");
    lbLambda.set_text("lambda");
    lbWeightm.set_text("weight-slope");
    lbWeightb.set_text("weight-offset");
    lbWeighttau.set_text("weight-tau");
    tfSlope.set_text("50.0");
    tfOffset.set_text("20.0");
    tfTau.set_text("5.0");
    tfLambda.set_text("0.01");
    tfWeightm.set_text("1.0");
    tfWeightb.set_text("0.5");
    tfWeighttau.set_text("0.1");
}

void MainWindow::on_window_resized ()
{
    // when you override any of the drawable_window events you have to make sure you
    // call the drawable_window's version of them because it needs to process
    // the events as well.  So we do that here.
    drawable_window::on_window_resized();

    // The rest of this function positions the widgets on the window
    unsigned long width,height;
    get_size(width,height);

    // Don't do anything if the user just made the window too small.  That is, leave
    // the widgets where they are.
    if (width < 500 || height < 350)
        return;

    // Set the size of the probability tables and the drawing area for the graph
    graph.set_size(0.97*width,0.55*height-+mbar.height());
    targetGrid.set_size(width-(recOptions.width()+recActions.width()+0.125*width),174);
    // tell the tabbed display to make itself just the right size to contain
    // the two probability tables.
    tabs.fit_to_contents();

    tfSlope.set_width(60);
    tfOffset.set_width(60);
    tfTau.set_width(60);
    tfLambda.set_width(60);
    tfWeightm.set_width(60);
    tfWeightb.set_width(60);
    tfWeighttau.set_width(60);

    btnLoadTextGrid.set_pos(255,graph.bottom()+30);
	btnLoadPitchTier.set_pos(255,graph.bottom()+60);
	btnOptimize.set_pos(255,graph.bottom()+90);
	btnStoreGesture.set_pos(255,graph.bottom()+120);
	btnStoreCsv.set_pos(255,graph.bottom()+150);
	btnClear.set_pos(255,graph.bottom()+180);
	targetGrid.set_pos(395,graph.bottom()+30);
	tabs.set_pos(23,graph.bottom()+30);

	btnLoadTextGrid.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
	btnLoadPitchTier.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
	btnOptimize.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
	btnStoreCsv.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());
	btnClear.set_size(btnStoreGesture.get_rect().width(), btnStoreGesture.get_rect().height());

    // Tell the named rectangle to position itself such that it fits around the
    // tabbed display that contains the probability tables and the label at the top of the
    // screen.
	recOptions.wrap_around(tabs.get_rect());
	recTargets.wrap_around(targetGrid.get_rect());
	recActions.wrap_around(btnLoadTextGrid.get_rect()+btnClear.get_rect());
}

void MainWindow::openTextGrid (const std::string& fileName)
{
    try
    {
		// process TextGrid input
		TextGridReader tgreader (fileName);
		m_bounds = tgreader.getBounds();
		graph.setBounds(m_bounds);
    }
    catch (std::exception& e)
    {
    	message_box("Error", "Unable to load TextGrid file " + fileName + "\n" + e.what());
    }
}

void MainWindow::openPitchTier (const std::string& fileName)
{
    try
    {
		// process PitchTier input
		PitchTierReader ptreader (fileName);
		m_origF0 = ptreader.getF0();
		graph.setOrigF0(m_origF0);
        set_title("Target Optimizer - " + left_substr(right_substr(fileName,"\\/"), "."));
    }
    catch (std::exception& e)
    {
        message_box("Error", "Unable to load PitchTier file " + fileName + "\n" + e.what());
    }
}

void MainWindow::onButtonTextGridOpen ()
{
    // display a file chooser window and when the user choses a file
    // call the on_open_file_selected() function
    open_existing_file_box(*this, &MainWindow::openTextGrid);
    if (!m_origF0.empty())
    {
    	onReadyForOptimize();
    }
}

void MainWindow::onButtonPitchTierOpen ()
{
    // display a file chooser window and when the user choses a file
    // call the on_open_file_selected() function
    open_existing_file_box(*this, &MainWindow::openPitchTier);
    if (!m_bounds.empty())
    {
    	onReadyForOptimize();
    }
}

void MainWindow::onReadyForOptimize ()
{
	btnOptimize.enable();
}

void MainWindow::onButtonOptimize ()
{
	// main task
	ParameterSet parameters;
	try
	{
		parameters = readParameters();
	}
	catch (std::exception& e)
	{
		std::string msg (e.what());
		message_box("Error", "Wrong option input \n" + msg);
		return;
	}

	OptimizationProblem problem (parameters, m_origF0, m_bounds);
	BobyqaOptimizer optimizer;
	optimizer.optimize(problem);
	m_optTarget = problem.getPitchTargets();
	m_optF0 = problem.getModelF0();
	m_optOnset = problem.getOnset();
	graph.setTargets(m_optTarget);
	graph.setOptimalF0(m_optF0);

	// show target values in grid
	targetGrid.set_grid_size(5,m_optTarget.size()+2);
    targetGrid.set_text(1,0,"slope [st/s]");
    targetGrid.set_text(2,0,"offset [st]");
    targetGrid.set_text(3,0,"tau [ms]");
    targetGrid.set_text(4,0,"duration [s]");
    targetGrid.set_background_color(1,0,rgb_pixel(200,250,250));
    targetGrid.set_background_color(2,0,rgb_pixel(200,250,250));
    targetGrid.set_background_color(3,0,rgb_pixel(200,250,250));
    targetGrid.set_background_color(4,0,rgb_pixel(200,250,250));
    targetGrid.set_text(0,0,"parameter");
    targetGrid.set_background_color(0,0,rgb_pixel(170,220,220));

	for (unsigned i=0; i<m_optTarget.size(); ++i)
	{
		std::ostringstream value; value << std::fixed << std::setprecision(0); value << i+1;
	    targetGrid.set_text(0,i+1,"target[" + value.str() + "]");
	    targetGrid.set_background_color(0,i+1,rgb_pixel(170,220,220));
	    value.str(""); value.clear(); value << std::fixed << std::setprecision(3);
	    value << m_optTarget[i].slope; targetGrid.set_text(1,i+1,value.str()); value.str(""); value.clear();
	    value << m_optTarget[i].offset; targetGrid.set_text(2,i+1,value.str()); value.str(""); value.clear();
	    value << m_optTarget[i].tau; targetGrid.set_text(3,i+1,value.str()); value.str(""); value.clear();
	    value << m_optTarget[i].duration; targetGrid.set_text(4,i+1,value.str()); value.str(""); value.clear();
	}

	// set non-editable
	for (unsigned i=0; i<targetGrid.number_of_rows(); ++i)
	{
		for (unsigned j=0; j<targetGrid.number_of_columns(); ++j)
		{
			targetGrid.set_editable(i,j,false);
		}
	}
	unblockMainWindow();
	std::ostringstream msg;
	msg << "Optimization successful!\nRMSE = " << problem.getRootMeanSquareError() << " st\nCORR = " << problem.getCorrelationCoefficient();
	message_box("Information", msg.str());
	//message_box_blocking("Information", msg.str());
}

ParameterSet MainWindow::readParameters()
{
	//calculate mean f0
	double meanF0 = 0.0;
	for (unsigned i=0; i<m_origF0.size(); ++i)
	{
		meanF0 += m_origF0[i].value;
	}
	meanF0 /= m_origF0.size();

	ParameterSet parameters;

	parameters.deltaSlope = atof(tfSlope.text().c_str());
	parameters.deltaOffset = atof(tfOffset.text().c_str());
	parameters.deltaTau = atof(tfTau.text().c_str());
	parameters.weightSlope = atof(tfWeightm.text().c_str());
	parameters.weightOffset = atof(tfWeightb.text().c_str());
	parameters.weightTau = atof(tfWeighttau.text().c_str());
	parameters.lambda = atof(tfLambda.text().c_str());
	parameters.meanSlope = 0.0;
	parameters.meanOffset = meanF0;
	parameters.meanTau = 15.0;

	// check input
	if (parameters.deltaSlope == 0.0)
		throw dlib::error("\"" + tfSlope.text() + "\" is no valid value for slope search space");
	if (parameters.deltaOffset == 0.0)
		throw dlib::error("\"" + tfOffset.text() + "\" is no valid value for offset search space");
	if (parameters.deltaTau == 0.0)
		throw dlib::error("\"" + tfTau.text() + "\" is no valid value for tau search space");

	if (parameters.weightSlope == 0.0 && !(!tfWeightm.text().compare("0.0") || !tfWeightm.text().compare("0")))
		throw dlib::error("\"" + tfWeightm.text() + "\" is no valid value for slope weight");

	if (parameters.weightOffset == 0.0 && !(!tfWeightb.text().compare("0.0") || !tfWeightb.text().compare("0")))
		throw dlib::error("\"" + tfWeightb.text() + "\" is no valid value for offset weight");

	if (parameters.weightTau == 0.0 && !(!tfWeighttau.text().compare("0.0") || !tfWeighttau.text().compare("0")))
		throw dlib::error("\"" + tfWeighttau.text() + "\" is no valid value for tau weight");

	if (parameters.lambda == 0.0 && !(!tfLambda.text().compare("0.0") || !tfLambda.text().compare("0")))
		throw dlib::error("\"" + tfLambda.text() + "\" is no valid value for lambda");

	// every other wrong input gets 0.0 due to atof()

	return parameters;
}

void MainWindow::onButtonClear ()
{
	// empty vectors
	BoundVector b; TimeSignal s; TargetVector t;

	// clear main window data
	m_bounds = b;
	m_origF0 = s;
	m_optF0 = s;

	graph.setBounds(b);
	graph.setOrigF0(s);
	graph.setOptimalF0(s);
	graph.setTargets(t);

	// clear text
	for (unsigned i=0; i<targetGrid.number_of_rows(); ++i)
	{
		for (unsigned j=0; j<targetGrid.number_of_columns(); ++j)
		{
			targetGrid.set_text(i,j,"");
		}
	}

	noOptimizationPerformed();
}

// This event is called when the user choses which file to save the graph to
void MainWindow::onSaveFileGesture (const std::string& fileName)
{
  std::string name = fileName;
  if (name.find(".ges") == std::string::npos)
  {
    name += ".ges";
  }
  GestureWriter gwriter (name);
  gwriter.writeTargets(m_optOnset, m_optTarget);
}

// This event is called when the user selects from the menu bar File->Save As
void MainWindow::onButtonSaveAsGesture ()
{
    save_file_box(*this, &MainWindow::onSaveFileGesture);
}

// This event is called when the user choses which file to save the graph to
void MainWindow::onSaveFileCsv (const std::string& fileName)
{
  std::string name = fileName;
  if (name.find(".csv") == std::string::npos)
  {
    name += ".csv";
  }
	CsvWriter cwriter (name);
	cwriter.writeTargets(m_optOnset, m_optTarget);
}

// This event is called when the user selects from the menu bar File->Save As
void MainWindow::onMenuSaveAsCsv ()
{
    save_file_box(*this, &MainWindow::onSaveFileCsv);
}

// This event is called when the user choses which file to save the graph to
void MainWindow::onSaveFilePitchTier(const std::string& fileName)
{
  std::string name = fileName;
  if (name.find(".PitchTier") == std::string::npos)
  {
    name += ".PitchTier";
  }
  PitchTierWriter pwriter(name);
	pwriter.writeF0(m_optF0);
}

// This event is called when the user selects from the menu bar File->Save As
void MainWindow::onMenuSaveAsPitchTier ()
{
    save_file_box(*this, &MainWindow::onSaveFilePitchTier);
}

void MainWindow::unblockMainWindow()
{
	tabs.enable();
	targetGrid.enable();
	btnLoadTextGrid.enable();
	btnLoadPitchTier.enable();
	btnOptimize.enable();
	btnClear.enable();
	btnStoreGesture.enable();
	btnStoreCsv.enable();
	graph.enable();
	mbar.enable();
}

void MainWindow::onMenuFileQuit()
{
	close_window();
}

void MainWindow::onMenuFileHelp()
{
	message_box("Help",
	                "(1) Load Praat-TextGrid file (Praat -> Save as short text file...).\n"
	                "(2) Load Praat-PitchTier file (Praat -> Save as PitchTier spreadsheet file...).\n"
					"(3) Choose options and start optimization.\n"
					"(4) Save targets as VTL gesture or csv file. Save modeled f0 as PitchTier file.\n");
}

void MainWindow::onMenuFileAbout()
{
    message_box("About","This application calculates the optimal Pitch Targets according to the \n"
                "Target-Approximation-Model utilizing a 5th order low pass filter.\n\n"
                "Version 1.0\n\n"
                "by Patrick Schmager");
}
