#include <fstream>
#include <dlib/string.h>
#include <dlib/error.h>
#include "TextGridReader.h"


TextGridReader::TextGridReader(const std::string& textGridFile)
{
	readFile(textGridFile);
}

BoundaryVector TextGridReader::getBounds() const
{
	return m_bounds;
}

void TextGridReader::readFile(const std::string& textGridFile)
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
	while (std::getline(fin, line))
	{
		if (line == "\"1\"")
		{
			m_bounds.push_back(atof(line_2.c_str()));
			m_bounds.push_back(atof(line_1.c_str()));
		}
		else if (!line.substr(0, 1).compare("\""))
		{
			line = dlib::trim(line, "\"");
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

bool TextGridReader::checkDigits(const std::string& s)
{
	return s.find_first_not_of("0123456789") == std::string::npos;
}