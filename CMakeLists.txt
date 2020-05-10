cmake_minimum_required(VERSION 3.2)

project(gg VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_COLOR_MAKEFILE ON)

# Enable C++17.
set(CMAKE_CXX_STANDARD 17)

# Debug mode for now
set(CMAKE_BUILD_TYPE Debug)

# Defines the variable SOURCES.
set(SOURCES
	src/main.cpp
	src/audioEngine.hpp src/audioEngine.cpp)

# Adds a new (executable) target called gg.
add_executable(geena ${SOURCES})

# Set compiler flags
target_compile_options(geena PRIVATE -Wall -Werror)

# Finds the required libraries. 
find_library(LIB_LIBSNDFILE    sndfile)
find_library(LIB_LIBSAMPLERATE samplerate)

# Fatal error if a library is missing from the system.
if (NOT LIB_LIBSNDFILE)
	message(FATAL_ERROR "libsndfile not found")
endif()
if (NOT LIB_LIBSAMPLERATE)
	message(FATAL_ERROR "libsamplerate not found")
endif()

# Specify libraries or flags to use when linking the target gg. These will 
# become linker flags such as -lSDL2 and -lSDL2_ttf.
target_link_libraries(geena sndfile samplerate)
