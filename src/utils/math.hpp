#pragma once


#include <type_traits>


namespace geena::utils::math
{
/* map
Maps 'x' in range [0, b) to a new range [0, z]. */

template <typename TI, typename TO>
TO map(TI x, TI b, TO z)
{
	static_assert(std::is_arithmetic_v<TI>);
	static_assert(std::is_arithmetic_v<TO>);
	
	return static_cast<TO>((x / (double) b) * z);
}
} // geena::utils::string 