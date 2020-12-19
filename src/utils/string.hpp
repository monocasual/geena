#pragma once


#include <string>
#include <vector>
#include <sstream>


namespace geena::utils::string 
{
std::string replace(std::string in, const std::string& search, 
	const std::string& replace);

std::string trim(const std::string& s);

std::vector<std::string> split(std::string in, std::string sep);
} // geena::utils::string 