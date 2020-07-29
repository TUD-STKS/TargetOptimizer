#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include "TextGrid.h"


TextGrid::TextGrid(std::string name)
{
	mark = name;
	n = 0;
	tmin = 0;
	tmax = 0;
}

int TextGrid::getNumberOfTiers()
{
	return this->n;
}

IntervalTier TextGrid::getIntervalTier(int i)
{
	return this->intervalTiers[i];
}

PointTier TextGrid::getPointTier(int i)
{
	return this->pointTiers[i];
}

double TextGrid::start()
{
	return this->tmin;
}

double TextGrid::end()
{
	return this->tmax;
}

std::string TextGrid::name()
{
	return this->mark;
}

void TextGrid::append(IntervalTier intervalTier)
{
	this->intervalTiers.push_back(intervalTier);
	this->tmax = std::max(this->tmax, intervalTier.end());
	this->tmin = std::min(this->tmin, intervalTier.start());
	this->n = getNumberOfTiers();
}

void TextGrid::append(PointTier pointTier)
{
	this->pointTiers.push_back(pointTier);
	this->tmax = std::max(this->tmax, pointTier.end());
	this->tmin = std::min(this->tmin, pointTier.start());
	this->n = getNumberOfTiers();
}

void TextGrid::changeOffset(double offset)
{
	this->tmin += offset;
	this->tmax += offset;
	for (auto&& intervalTier : this->intervalTiers) {
		intervalTier.changeOffset(offset);
	}
	for (auto&& pointTier : this->pointTiers) {
		pointTier.changeOffset(offset);
	}
}

void TextGrid::changeTimes(double start, double end)
{
	this->tmin = start;
	this->tmax = end;
}

void TextGrid::textGridReader(std::string inputFilename)
{
	using namespace std;
	ifstream inputFile;
	try {
		inputFile.open(inputFilename);
		if (inputFile.is_open()) {
			string line;
			for (int lineCnt = 1; lineCnt < 4; lineCnt++) {
				getline(inputFile, line);
			}
			getline(inputFile, line);
			vector<string> lineElements = split(trim(line));
			if (lineElements.size() == 3 && lineElements[0] == "xmin") {
				textGridFormat = "long";
				cout << "TextGrid is long!" << endl;
			}
			else if (lineElements.size() == 1 && lineElements[0] != "" ) {
				textGridFormat = "short";
				cout << "TextGrid is short!" << endl;
			}
			else {
				cout << "TextGridFile has an unknown file format!" << endl;
			}
			this->tmin = stod(lineElements[0].c_str());
			getline(inputFile, line);
			this->tmax = stod(rtrim(line).c_str());
			getline(inputFile, line);
			getline(inputFile, line);
			this->n = stoi(rtrim(line).c_str());
			for (int tierNumber = 1; tierNumber <= this->n; tierNumber++) {
				getline(inputFile, line);
				getline(inputFile, line);
				if (trim(regex_replace(line, regex("\""), "\t")) == "IntervalTier") {
					getline(inputFile, line);
					string tierName = trim(regex_replace(line, regex("\""), "\t"));
					getline(inputFile, line);
					double tierMin = stod(trim(line).c_str());
					getline(inputFile, line);
					double tierMax = stod(trim(line).c_str());
					IntervalTier tmpTier = IntervalTier(tierMin, tierMax, tierName);
					getline(inputFile, line);
					tmpTier.n = stoi(rtrim(line).c_str());
					for (int intervalNumber = 1; intervalNumber <= tmpTier.n; intervalNumber++) {
						getline(inputFile, line);
						double intervalMin = stod(trim(line).c_str());
						getline(inputFile, line);
						double intervalMax = stod(trim(line).c_str());
						getline(inputFile, line);
						string intervalMark = trim(regex_replace(line, regex("\""), "\t"));
						tmpTier.append(Interval(intervalMin, intervalMax, intervalMark));
					}
					this->append(tmpTier);
					int a = 10;
				}
				
			}

			inputFile.close();
		}
	}
	catch (ifstream::failure e) {
		cerr << "Exception opening/reading/closing file!\n";
	}
}

void TextGrid::textGridWriter(std::string outputFilename)
{
	int dummy = 10;
}

std::vector<std::string> TextGrid::split(std::string stringFromLine)
{
	int element = 0;
	std::vector<std::string> tmpLineElements;
	tmpLineElements.push_back("");
	for (auto&& character : stringFromLine) {
		if (!isspace(character)) {
			tmpLineElements[element] += character;
		}
		else {
			if (isspace(character) && tmpLineElements[element] != "") {
				tmpLineElements.push_back("");
				element++;
			}
		}
	}
	return tmpLineElements;
}
