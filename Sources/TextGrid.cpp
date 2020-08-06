#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include "StringHelper.h"
#include "TextGrid.h"

TextGrid TextGrid::readTextGridFile(const std::string& inputFilename)
{
	using namespace std;
	ifstream inputFile;
	try {
		inputFile.open(inputFilename);
		inputFile.exceptions(inputFile.failbit);
		if (inputFile.is_open()) {
			string line;
			// header
			for (int lineCnt = 1; lineCnt <= 2; lineCnt++) {
				getline(inputFile, line);
			}
			// empty lines
			getline(inputFile, line);
			while (line == "") {
				getline(inputFile, line);
			}
			vector<string> lineElements = split(trim(line));
			if (lineElements.size() == 3 && lineElements[0] == "xmin") {
				TextGrid tg = LongTextGridFactory(inputFile, lineElements);
				cout << "TextGridFile has format long!" << endl;
				return tg;
			}
			else if (lineElements.size() == 1 && lineElements[0] != "") {
				TextGrid tg = ShortTextGridFactory(inputFile, lineElements);
				cout << "TextGridFile has format short!" << endl;
				return tg;
			}
		}
	}
	catch (const ios_base::failure& e) {
		cerr << "Exception opening/reading/closing file! " << e.what() << endl;
	}
}

bool TextGrid::writeTextGridFile(TextGrid& tg, const std::string& outputFilename, const std::string& format)
{
	using namespace std;
	ofstream outputFile;
	try {
		outputFile.open(outputFilename);
		outputFile.exceptions(outputFile.failbit);
		if (outputFile.is_open()) {
			string line;
			outputFile << "File type = \"ooTextFile\"" << endl;
			outputFile << "Object class = \"TextGrid\"" << endl;
			outputFile << endl;
			if (format == "short") {
				outputFile << tg.getStart() << endl;
				outputFile << tg.getEnd() << endl;
				outputFile << "<exists>" << endl;
				outputFile << tg.intervalTiers.size() + tg.pointTiers.size() << endl;
				for (auto& intervalTier : tg.intervalTiers) {
					outputFile << "\"IntervalTier\"" << endl;
					outputFile << "\"" + intervalTier.first + "\"" << endl;
					outputFile << intervalTier.second.getStartingTime() << endl;
					outputFile << intervalTier.second.getEndingTime() << endl;
					outputFile << intervalTier.second.size() << endl;
					for (int intervalNumber = 0; intervalNumber < intervalTier.second.size(); intervalNumber++) {
						outputFile << intervalTier.second[intervalNumber].getStart() << endl;
						outputFile << intervalTier.second[intervalNumber].getEnd() << endl;
						outputFile << "\"" + intervalTier.second[intervalNumber].text + "\"" << endl;
					}
				}
				for (auto& pointTier : tg.pointTiers) {
					outputFile << "\"TextTier\"" << endl;
					outputFile << "\"" + pointTier.first + "\"" << endl;
					outputFile << pointTier.second.getStartingTime() << endl;
					outputFile << pointTier.second.getEndingTime() << endl;
					outputFile << pointTier.second.size() << endl;
					for (int pointNumber = 0; pointNumber < pointTier.second.size(); pointNumber++) {
						outputFile << pointTier.second[pointNumber].getStart() << endl;
						outputFile << "\"" + pointTier.second[pointNumber].mark + "\"" << endl;
					}
				}
				outputFile.close();
				return true;
			}
			else if (format == "long") {
				outputFile << "xmin = " << tg.getStart() << endl;
				outputFile << "xmax = " << tg.getEnd() << endl;
				outputFile << "tiers? <exists>" << endl;
				outputFile << "size = " << tg.intervalTiers.size() + tg.pointTiers.size() << endl;
				outputFile << "item []:" << endl;
				int tierCnt = 1;
				for (auto& intervalTier : tg.intervalTiers) {
					outputFile << "    item [" + to_string(tierCnt) + "]:" << endl;
					tierCnt += 1;
					outputFile << "        class = \"IntervalTier\"" << endl;
					outputFile << "        name = " + (string)"\"" + intervalTier.first + "\"" << endl;
					outputFile << "        xmin = " + to_string(intervalTier.second.getStartingTime()) << endl;
					outputFile << "        xmax = " + to_string(intervalTier.second.getEndingTime()) << endl;
					outputFile << "        intervals: size = " + to_string(intervalTier.second.size()) << endl;
					for (int intervalNumber = 0; intervalNumber < intervalTier.second.size(); intervalNumber++) {
						outputFile << "        intervals [" + to_string(intervalNumber + 1) + "]:" << endl;
						outputFile << "            xmin = " + to_string(intervalTier.second[intervalNumber].getStart()) << endl;
						outputFile << "            xmax = " + to_string(intervalTier.second[intervalNumber].getEnd()) << endl;
						outputFile << "            text = \"" + intervalTier.second[intervalNumber].text + "\"" << endl;
					}
				}
				for (auto& pointTier : tg.pointTiers) {
					outputFile << "    item [" + to_string(tierCnt) + "]:" << endl;
					tierCnt += 1;
					outputFile << "        class = \"TextTier\"" << endl;
					outputFile << "        name = " + (string)"\"" + pointTier.first + "\"" << endl;
					outputFile << "        xmin = " + to_string(pointTier.second.getStartingTime()) << endl;
					outputFile << "        xmax = " + to_string(pointTier.second.getEndingTime()) << endl;
					outputFile << "        intervals: size = " + to_string(pointTier.second.size()) << endl;
					for (int pointNumber = 0; pointNumber < pointTier.second.size(); pointNumber++) {
						outputFile << "        points [" + to_string(pointNumber + 1) + "]:" << endl;
						outputFile << "            number = " + to_string(pointTier.second[pointNumber].getStart()) << endl;
						outputFile << "            mark = \"" + pointTier.second[pointNumber].mark + "\"" << endl;
					}
				}
				outputFile.close();
				return true;
			}
			else { // binary

			}			
		}
	}
	catch (const ios_base::failure& e) {
		cerr << "Exception opening/writing/closing file! " << e.what() << endl;
	}
	return false;
}

void TextGrid::appendIntervalTier(IntervalTier& intervalTier)
{
	this->intervalTiers.insert(std::make_pair(intervalTier.name, intervalTier));
	std::cout << "IntervalTier has successfully been added!" << std::endl;
}

void TextGrid::appendPointTier(PointTier& pointTier)
{
	this->pointTiers.insert(std::make_pair(pointTier.name, pointTier));
	std::cout << "PointTier has successfully been added!" << std::endl;
}

IntervalTier& TextGrid::getIntervalTier(std::string name)
{
	return this->intervalTiers.find(name)->second;
}

PointTier& TextGrid::getPointTier(std::string name)
{
	return this->pointTiers.find(name)->second;
}

double TextGrid::getStart()
{
	double intervalMin = INFINITY;
	double pointMin = INFINITY;
	double tierMin = INFINITY;

	if (this->intervalTiers.size() > 0) {
		intervalMin = this->intervalTiers.begin()->second.getStartingTime();
		for (auto iterator = intervalTiers.begin(); iterator != intervalTiers.end(); ++iterator) {
			if (iterator->second.getStartingTime() < intervalMin) {
				intervalMin = iterator->second.getStartingTime();
			}
		}
	}	

	if (this->pointTiers.size() > 0) {
		pointMin = this->pointTiers.begin()->second.getStartingTime();
		for (auto iterator = pointTiers.begin(); iterator != pointTiers.end(); ++iterator) {

			if (iterator->second.getStartingTime() < pointMin) {
				pointMin = iterator->second.getStartingTime();
			}
		}
	}

	if (intervalMin <= pointMin) {
		tierMin = intervalMin;
	}
	else
	{
		tierMin = pointMin;
	}

	return tierMin;
}

double TextGrid::getEnd()
{
	double intervalMax = 0;
	double pointMax = 0;
	double tierMax = 0;

	if (this->intervalTiers.size() > 0) {
		intervalMax = this->intervalTiers.begin()->second.getEndingTime();
		for (auto iterator = intervalTiers.begin(); iterator != intervalTiers.end(); ++iterator) {
			if (iterator->second.getEndingTime() > intervalMax) {
				intervalMax = iterator->second.getEndingTime();
			}
		}
	}
	
	if (this->pointTiers.size() > 0 ) {
		pointMax = this->pointTiers.begin()->second.getEndingTime();
		for (auto iterator = pointTiers.begin(); iterator != pointTiers.end(); ++iterator) {
			if (iterator->second.getEndingTime() > pointMax) {
				pointMax = iterator->second.getEndingTime();
			}
		}
	}
	

	if (intervalMax >= pointMax) {
		tierMax = intervalMax;
	}
	else
	{
		tierMax = pointMax;
	}

	return tierMax;
}


TextGrid TextGrid::BinaryTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements)
{
	TextGrid tgb = TextGrid();

	return tgb;
}

TextGrid TextGrid::ShortTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements)
{
	using namespace std;
	string line;

	TextGrid tgs = TextGrid();
	// TMIN
	//double tmin = stod(lineElements[0].c_str());
	// Empty Lines
	getline(file, line);
	while (line == "") {
		getline(file, line);
	}
	// TMAX
	//double tmax = stod(rtrim(line).c_str());
	// Empty Lines
	getline(file, line);
	while (line == "") {
		getline(file, line);
	}
	// <exists>
	getline(file, line);
	// Empty Lines
	while (line == "") {
		getline(file, line);
	}
	// NUMBER OF TIERS
	int numberOfTiersToAdd = stoi(rtrim(line).c_str());
	for (int tierNumber = 1; tierNumber <= numberOfTiersToAdd; tierNumber++) {
		getline(file, line);
		while (line == "") {
			getline(file, line);
		}
		// IntervalTier
		if (trim(regex_replace(line, regex("\""), "\t")) == "IntervalTier") {
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			string tierName = trim(regex_replace(line, regex("\""), "\t"));
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMin = stod(trim(line).c_str());
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMax = stod(trim(line).c_str());
			IntervalTier tmpTier = IntervalTier(tierName);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			// NUMBER OF INTERVALS IN TIER
			int numberOfIntervalsToAdd = stoi(rtrim(line).c_str());
			for (int intervalNumber = 1; intervalNumber <= numberOfIntervalsToAdd; intervalNumber++) {
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// ITMIN
				double intervalMin = stod(trim(line).c_str());
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// ITMAX
				double intervalMax = stod(trim(line).c_str());
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// ITEXT
				string intervalText = trim(regex_replace(line, regex("\""), "\t"));
				tmpTier.append(Interval(intervalMin, intervalMax, intervalText));
			}
			tgs.appendIntervalTier(tmpTier);
		}
		// PointTier
		else if (trim(regex_replace(line, regex("\""), "\t")) == "TextTier") {
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			string tierName = trim(regex_replace(line, regex("\""), "\t"));
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			//double tierMin = stod(trim(line).c_str());
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMax = stod(trim(line).c_str());
			PointTier tmpTier = PointTier(tierName);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			// NUMBER OF POINTS IN TIER
			int numberOfPointsToAdd = stoi(rtrim(line).c_str());
			for (int pointN = 1; pointN <= numberOfPointsToAdd; pointN++) {
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// PNUMBER
				double pointNumber = stod(trim(line).c_str());
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// PMARK
				string pointMark = trim(regex_replace(line, regex("\""), "\t"));
				tmpTier.append(Point(pointNumber, pointMark));
			}
			tgs.appendPointTier(tmpTier);
		}
	}
	file.close();
	return tgs;
}

TextGrid TextGrid::LongTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements)
{
	using namespace std;
	string line;

	TextGrid tgl = TextGrid();
	// TMIN
	//double tmin = stod(lineElements[2].c_str());
	// Empty Lines
	getline(file, line);
	while (line == "") {
		getline(file, line);
	}
	// TMAX
	lineElements = split(trim(line), "=");
	//double tmax = stod(lineElements[2].c_str());
	// Empty Lines
	getline(file, line);
	while (line == "") {
		getline(file, line);
	}
	// <exists>
	getline(file, line);
	// Empty Lines
	while (line == "") {
		getline(file, line);
	}
	// NUMBER OF TIERS
	int numberOfTiersToAdd = stoi(split(trim(line).c_str(), "=")[2]);
	// item []:
	getline(file, line);
	// Empty Lines
	while (line == "") {
		getline(file, line);
	}
	for (int tierNumber = 1; tierNumber <= numberOfTiersToAdd; tierNumber++) {
		// item [x]:
		getline(file, line);
		while (line == "") {
			getline(file, line);
		}
		getline(file, line);
		// IntervalTier
		if (split(trim(line).c_str(), "=")[2] == "\"IntervalTier\"") {
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			string tierName = split(trim(regex_replace(line, regex("\""), "\t")), "=")[2];
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMin = stod(split(trim(line).c_str(), "=")[2]);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMax = stod(split(trim(line).c_str(), "=")[2]);
			IntervalTier tmpTier = IntervalTier(tierName);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			int numberOfIntervalsToAdd = stoi(split(trim(line).c_str(), "=")[2]);
			for (int intervalNumber = 1; intervalNumber <= numberOfIntervalsToAdd; intervalNumber++) {
				// intervals [x]:
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// IMIN
				double intervalMin = stod(split(trim(line).c_str(), "=")[2]);
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// IMAX
				double intervalMax = stod(split(trim(line).c_str(), "=")[2]);
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// ITEXT
				string intervalText = split(trim(regex_replace(line, regex("\""), "\t")), "=")[2];
				tmpTier.append(Interval(intervalMin, intervalMax, intervalText));
			}
			tgl.appendIntervalTier(tmpTier);
		}
		// PointTier
		else if (split(trim(line).c_str(), "=")[2] == "\"TextTier\"") {
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			string tierName = split(trim(regex_replace(line, regex("\""), "\t")), "=")[2];
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			//double tierMin = stod(split(trim(line).c_str(), "=")[2]);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			//double tierMax = stod(split(trim(line).c_str(), "=")[2]);
			PointTier tmpTier = PointTier(tierName);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			int numberOfPointsToAdd = stoi(split(trim(line).c_str(), "=")[2]);
			for (int pointN = 1; pointN <= numberOfPointsToAdd; pointN++) {
				// points [x]:
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// PNUMBER
				double pointNumber = stod(split(trim(line).c_str(), "=")[2]);
				getline(file, line);
				while (line == "") {
					getline(file, line);
				}
				// PMARK
				string pointMark = split(trim(regex_replace(line, regex("\""), "\t")), "=")[2];
				tmpTier.append(Point(pointNumber, pointMark));
			}
			tgl.appendPointTier(tmpTier);
		}
	}
	file.close();
	return tgl;
}

