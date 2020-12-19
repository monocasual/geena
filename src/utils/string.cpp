#include "string.hpp"


namespace geena::utils::string 
{
std::string trim(const std::string& s)
{
	std::size_t first = s.find_first_not_of(" \n\t");
	std::size_t last  = s.find_last_not_of(" \n\t");
	return s.substr(first, last-first+1);
}


/* -------------------------------------------------------------------------- */


std::string replace(std::string in, const std::string& search, const std::string& replace)
{
	std::size_t pos = 0;
	while ((pos = in.find(search, pos)) != std::string::npos) {
		in.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return in;
}


/* -------------------------------------------------------------------------- */


std::vector<std::string> split(std::string in, std::string sep)
{
	std::vector<std::string> out;
	std::string full  = in;
	std::string token = "";
	std::size_t curr = 0;
	std::size_t next = -1;
	do {
		curr  = next + 1;
		next  = full.find_first_of(sep, curr);
		token = full.substr(curr, next - curr);
		if (token != "")
			out.push_back(token);
	}
	while (next != std::string::npos);
	return out;
}
} // geena::utils::string 
