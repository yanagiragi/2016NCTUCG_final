#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

namespace Utils{

	template<typename Out> void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}
};

#endif // !_UTILS_HPP