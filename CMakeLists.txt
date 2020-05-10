cmake_minimum_required(VERSION 3.2)

project(gg VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_COLOR_MAKEFILE ON)

# Enable C++17.
set(CMAKE_CXX_STANDARD 17)

# Debug mode for now
set(CMAKE_BUILD_TYPE Debug)

# Set include (-I) flag
# TODO - is this modern CMake?
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_SOURCE_DIR}/src")

# Defines the variable SOURCES.
set(SOURCES
	src/main.cpp
	src/audioEngine.hpp src/audioEngine.cpp
	src/deps/rtaudio/RtAudio.h src/deps/rtaudio/RtAudio.cpp)

# Adds a new (executable) target called gg.
add_executable(geena ${SOURCES})

# Set compiler flags
target_compile_options(geena PRIVATE -Wall)

# Set macro definitions (-D flags)
add_compile_definitions(__LINUX_ALSA__ __LINUX_PULSE__ __UNIX_JACK__)

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

# Specify libraries or flags to use when linking the target geena. These will 
# become linker flags such as -ljack.
target_link_libraries(geena jack asound pthread pulse pulse-simple)
