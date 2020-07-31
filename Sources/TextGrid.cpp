#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <stdlib.h>
#include "TextGrid.h"
#include "StringHelper.h"


TextGrid::TextGrid(double xmin, double xmax, int nTiers)
{
	tmin = xmin;
	tmax = xmax;
	numberOfTiers = nTiers;
}

TextGrid TextGrid::readTextGridFile(const std::string& inputFilename)
{
	using namespace std;
	ifstream inputFile;
	try {
		inputFile.open(inputFilename);
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
			else {
				TextGrid tg = BinaryTextGridFactory(inputFile, lineElements);
				cout << "TextGridFile has unknown format, probably binary!" << endl;
				return tg;
			}
		}
	}
	catch (ifstream::failure e) {
		cerr << "Exception opening/reading/closing file! " << e.what() << endl;
	}
}

TextGrid TextGrid::BinaryTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements)
{
	TextGrid tgb = TextGrid(0, 0, 0);

	return tgb;
}

TextGrid TextGrid::ShortTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements)
{
	using namespace std;
	string line;

	TextGrid tgs = TextGrid(0, 0, 0);
	// TMIN
	tgs.tmin = stod(lineElements[0].c_str());
	// Empty Lines
	getline(file, line);
	while (line == "") {
		getline(file, line);
	}
	// TMAX
	tgs.tmax = stod(rtrim(line).c_str());
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
			IntervalTier tmpTier = IntervalTier(tierMin, tierMax, tierName);
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
			tgs.append(tmpTier);
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
			double tierMin = stod(trim(line).c_str());
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMax = stod(trim(line).c_str());
			PointTier tmpTier = PointTier(tierMin, tierMax, tierName);
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
			tgs.append(tmpTier);
		}
	}
	file.close();
	return tgs;
}

TextGrid TextGrid::LongTextGridFactory(std::ifstream& file, std::vector<std::string> lineElements)
{
	using namespace std;
	string line;

	TextGrid tgl = TextGrid(0, 0, 0);
	// TMIN
	tgl.tmin = stod(lineElements[2].c_str());
	// Empty Lines
	getline(file, line);
	while (line == "") {
		getline(file, line);
	}
	// TMAX
	lineElements = split(trim(line), "=");
	tgl.tmax = stod(lineElements[2].c_str());
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
			IntervalTier tmpTier = IntervalTier(tierMin, tierMax, tierName);
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
			tgl.append(tmpTier);
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
			double tierMin = stod(split(trim(line).c_str(), "=")[2]);
			getline(file, line);
			while (line == "") {
				getline(file, line);
			}
			double tierMax = stod(split(trim(line).c_str(), "=")[2]);
			PointTier tmpTier = PointTier(tierMin, tierMax, tierName);
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
			tgl.append(tmpTier);
		}
	}

	return tgl;
}

void TextGrid::append(IntervalTier intervalTier)
{
	this->tierElements.emplace_back(intervalTier);
	this->tmax = std::max(this->tmax, intervalTier.getEndingTime());
	this->tmin = std::min(this->tmin, intervalTier.getStartingTime());
	this->numberOfTiers = getNumberOfTiers();
}

void TextGrid::append(PointTier pointTier)
{
	this->tierElements.emplace_back(pointTier);
	this->tmax = std::max(this->tmax, pointTier.getEndingTime());
	this->tmin = std::min(this->tmin, pointTier.getStartingTime());
	this->numberOfTiers = getNumberOfTiers();
}

int TextGrid::getNumberOfTiers()
{
	return this->tierElements.size();
}
