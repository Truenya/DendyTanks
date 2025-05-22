cmake_minimum_required(VERSION 3.20)
project(ssdl)

#settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debug build flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsized-deallocation -Wall -Wextra -Wpedantic -DDEBUG")

# Release build flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fsized-deallocation -Wall -Wextra -Wpedantic")

# Common settings
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Check for Conan package manager
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    # Try to find Conan
    find_program(CONAN_COMMAND conan)
    
    if(CONAN_COMMAND)
        # Platform-specific setup
        if(WIN32)
            message(STATUS "Windows platform detected")
        elseif(UNIX)
            message(STATUS "Unix platform detected")
        elseif(${CMAKE_SYSTEM} MATCHES "Darwin")
            message(STATUS "macOS platform detected")
            # Install Conan if needed on macOS
            execute_process(COMMAND pip install conan)
        endif()
        
        # Install dependencies using Conan
        message(STATUS "Installing dependencies with Conan")
        execute_process(
            COMMAND ${CONAN_COMMAND} install .. --install-folder . --build=missing
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        
        # Include the generated Conan file if it exists now
        if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
            include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
            conan_basic_setup()
        else()
            message(WARNING "The file conanbuildinfo.cmake doesn't exist at ${CMAKE_BINARY_DIR}, you may need to run conan install manually")
        endif()
    else()
        message(STATUS "Conan not found, using system libraries")
    endif()
endif()
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
