#include "fs.hpp"
#include "string.hpp"

namespace geena::utils::fs
{
std::string uriToPath(const std::string& uri)
{
	std::string out = uri;
	out             = string::replace(out, "file://", "");
	out             = string::replace(out, "%20", " ");
	return out;
}
} // namespace geena::utils::fs