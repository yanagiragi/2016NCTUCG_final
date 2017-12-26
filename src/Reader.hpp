#ifndef  _READER_HPP
#define _READER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Reader 
{
	std::ifstream infile;
	std::vector<std::string> line;
	std::string lineBuffer;

	public:
		Reader(const std::string filename)
		{
			infile = std::ifstream(filename);
			lineBuffer.clear();
			line.clear();

			if (infile.bad()) {
				std::cout << filename << " does not exists." << std::endl;
				return;
			}
			else {
				std::cout << filename << " is OK." << std::endl;
			}

			std::string buffer;
			while (std::getline(infile, buffer))
			{
				line.push_back(buffer);
				lineBuffer += buffer + '\n';
			}
		}

		std::vector<std::string> getLine()
		{
			return line;
		}

		std::string getLineBuffer()
		{
			return lineBuffer;
		}
};

#endif // ! _READER_HPP