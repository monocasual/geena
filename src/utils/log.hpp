#pragma once


#include <iostream>


namespace geena::utils 
{
#ifndef NDEBUG
    #define G_DEBUG_MODE
    #define G_DEBUG(x) std::cerr << __FILE__ << "::" << __func__  << "() - " << x << "\n";
#else
    #define G_DEBUG(x) do {} while (0)
#endif
} // geena::utils::string 