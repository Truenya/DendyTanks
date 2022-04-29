cmake_minimum_required(VERSION 3.20)
project(ssdl)

#libs
INCLUDE(FindPkgConfig)
pkg_check_modules(SDL REQUIRED sdl2)
pkg_check_modules(tcmalloc REQUIRED libtcmalloc)
pkg_check_modules(gtest REQUIRED gtest)

#settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -O0 -std=c++2a -fsized-deallocation -fprofile-generate ")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB mIncludes ./*.h )
file(GLOB mSources ./*.cpp )

file(GLOB mProcessorsIncludes Processors/*.h )
file(GLOB mProcessorsSources Processors/*.cpp )
file(GLOB_RECURSE mRecIncludes Entities/*.h)
file(GLOB_RECURSE mRecSources Entities/*.cpp)
message("Headers: ${mIncludes};${mRecIncludes}\nSources: ${mSources};${mRecSources}\n")
#includes
include_directories(${tcmalloc_INCLUDE_DIR})

#results
add_executable(ssdl ${mIncludes} ${mSources} ${mSources} ${mRecSources} ${mProcessorsIncludes} ${mProcessorsSources})

#linking
target_link_libraries(ssdl tcmalloc SDL2 SDL2_image gtest)
