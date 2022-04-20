# ------------------------------------------------------------------------------
# Preliminary setup
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.12)

# ------------------------------------------------------------------------------
# Project
# ------------------------------------------------------------------------------

project(geena LANGUAGES CXX)

# ------------------------------------------------------------------------------
# Lists definition
#
# SOURCES - contains the source files
# PREPROCESSOR_DEFS - preprocessor definitions
# INCLUDE_DIRS - include directories (e.g. -I)
# COMPILER_OPTIONS - additional flags for the compiler
# LIBRARIES - external dependencies to link
# COMPILER_FEATURES - e.g. C++17
# TARGET_PROPERTIES - additional properties for the final target.
# ------------------------------------------------------------------------------

list(APPEND SOURCES
	src/main.cpp
	src/ui/mainWindow.cpp
	src/ui/counter.cpp
	src/ui/progress.cpp
	src/ui/pitchSlider.cpp
	src/core/engine.cpp
	src/core/api.cpp
	src/core/rendering.cpp
	src/core/kernel.cpp
	src/core/audioFile.cpp
	src/core/audioFileFactory.cpp
	src/core/state.cpp
	src/deps/mcl-utils/src/fs.cpp
	src/deps/mcl-utils/src/string.cpp
	src/deps/audio-buffer/src/audioBuffer.cpp)
list(APPEND PREPROCESSOR_DEFS 
	__LINUX_ALSA__ 
	__LINUX_PULSE__ 
	__UNIX_JACK__) # TODO customize
list(APPEND INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src)
list(APPEND COMPILER_OPTIONS)
list(APPEND LIBRARIES stdc++fs)
list(APPEND COMPILER_FEATURES cxx_std_17)
list(APPEND TARGET_PROPERTIES)

# ------------------------------------------------------------------------------
# Detect OS
# ------------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(OS_LINUX 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(OS_WINDOWS 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(OS_MACOS 1)
else()
	message(FATAL_ERROR "Unsupported platform '${CMAKE_SYSTEM_NAME}', quitting.")
endif()

# ------------------------------------------------------------------------------
# Compiler setup
# ------------------------------------------------------------------------------

if(DEFINED OS_WINDOWS)
	list(APPEND COMPILER_OPTIONS /W4)
else()
	list(APPEND COMPILER_OPTIONS -Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	list(APPEND PREPROCESSOR_DEFS NDEBUG)
endif()

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE not defined!")
endif()

find_package(SndFile CONFIG REQUIRED)
list(APPEND LIBRARIES SndFile::sndfile)

find_package(RtAudio CONFIG REQUIRED)
list(APPEND LIBRARIES RtAudio::rtaudio)

find_package(FLTK CONFIG REQUIRED)
list(APPEND LIBRARIES fltk fltk_gl fltk_forms fltk_images)

# TODO - vcpkg currently doesn't provide CMake targets for libsamplerate: 
# let's include it the manual way. 

find_library(LIBRARY_SAMPLERATE 
	NAMES samplerate libsamplerate libsamplerate-0 liblibsamplerate-0
	PATH ${_VCPKG_ROOT_DIR}/installed/${VCPKG_TARGET_TRIPLET})
list(APPEND LIBRARIES ${LIBRARY_SAMPLERATE})


# ------------------------------------------------------------------------------
# Finalize 'geena' target (main executable).
# ------------------------------------------------------------------------------

add_executable(geena)
target_compile_features(geena PRIVATE ${COMPILER_FEATURES})
target_sources(geena PRIVATE ${SOURCES})
target_compile_definitions(geena PRIVATE ${PREPROCESSOR_DEFS})
target_include_directories(geena PRIVATE ${INCLUDE_DIRS})
target_link_libraries(geena PRIVATE ${LIBRARIES})
target_compile_options(geena PRIVATE ${COMPILER_OPTIONS})