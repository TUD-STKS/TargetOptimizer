#include <string>
#include <regex>


std::string ltrim(const std::string& s) {
	return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}
std::string rtrim(const std::string& s) {
	return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}
std::string trim(const std::string& s) {
	return ltrim(rtrim(s));
}

std::vector<std::string> split(std::string stringFromLine, std::string sep = "") {
	int element = 0;
	std::vector<std::string> tmpLineElements;
	tmpLineElements.push_back("");

	if (sep == "") {
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
	}
	else {
		for (auto&& character : stringFromLine) {
			if ((int)character < 0) {
				continue;
			}
			if (character != sep[0] && !isspace(character)) {
				tmpLineElements[element] += character;
			}
			else {
				if (character == sep[0] && tmpLineElements[element] != "") {
					tmpLineElements.push_back(sep);
					tmpLineElements.push_back("");
					element += 2;
				}
			}
		}
	}
	
	
	return tmpLineElements;
}