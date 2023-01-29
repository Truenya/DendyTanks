cmake_minimum_required(VERSION 3.20)
project(ssdl)

#settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -O0 -std=c++2a -fsized-deallocation -Wall -Wextra -Wpedantic")
#set(CMAKE_CXX_FLAGS "-g -O0 -std=c_++2a -fsized-deallocation -fprofile-use="1.profraw" -Wall -Wextra -Wpedantic")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#	conan_basic_setup()
#else()
#	execute_process(COMMAND which conan)
#	if (WIN32)
#		message (FATAL_ERROR "в **пу винду, сами разбирайтесь")
#	elseif (UNIX)
#	elseif (${CMAKE_SYSTEM} MATCHES "Darwin")
#		execute_process(COMMAND pip install conan)
#	endif ()
#	execute_process(COMMAND conan install .. --install-folder . --build=missing)
#	message(WARNING "The file conanbuildinfo.cmake doesn't exist at ${CMAKE_BINARY_DIR}, you have to run conan install first")
#endif()
#libs
INCLUDE(FindPkgConfig)
pkg_check_modules(SDL REQUIRED sdl2)
pkg_check_modules(uuid REQUIRED uuid)
#pkg_check_modules(tcmalloc REQUIRED libtcmalloc)
#pkg_check_modules(gtest REQUIRED gtest)



#let's collect files into the variables and use them instead
file(GLOB mIncludes ./*.h )
file(GLOB mSources ./*.cpp )
file(GLOB_RECURSE mProcessorsIncludes Processors/*.h )
file(GLOB_RECURSE mProcessorsSources Processors/*.cpp )
file(GLOB_RECURSE mEntityIncludes Entities/*.h)
file(GLOB_RECURSE mEntitySources Entities/*.cpp)
message("Headers:\t$${mEntityIncludes};\n\t${mProcessorsIncludes}\n\
Sources:\t${mSources};\n\t${mEntitySources};\n\t${mProcessorsSources}\n")

#includes
include_directories(${tcmalloc_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

#options
#currently is not implemented yet
unset(logging_game CACHE)
option(logging_game "Create log file and write all to it" OFF)
if (logging_game)
	add_definitions("-DMAKE_LOG")
	message("-- Logging is on")
	set(logging_game ON)
else()
	message("-- Logging is off")
endif (logging_game)

#results
add_executable(ssdl ${mIncludes} ${mSources} ${mEntityIncludes} ${mEntitySources} ${mProcessorsIncludes} ${mProcessorsSources} Processors/Common/NpcProcessor.cpp Processors/Common/NpcProcessor.h)

#linking
target_link_libraries(ssdl  SDL2 SDL2_test SDL2_image uuid)# tcmalloc gtest)
