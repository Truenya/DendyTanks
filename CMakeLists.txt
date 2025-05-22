cmake_minimum_required(VERSION 3.20)
project(ssdl)

#settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -O0 -std=c++2a -fsized-deallocation -Wall -Wextra -Wpedantic")
#set(CMAKE_CXX_FLAGS "-g -O0 -std=c_++2a -fsized-deallocation -fprofile-use="1.profraw" -Wall -Wextra -Wpedantic")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set dependency management variables
set(USE_CONAN FALSE)
set(USE_VCPKG FALSE)
set(USE_PKG_CONFIG FALSE)

# Check for Conan
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(STATUS "Using existing Conan configuration")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    set(USE_CONAN TRUE)
else()
    # Check if Conan is installed
    execute_process(
        COMMAND conan --version
        RESULT_VARIABLE CONAN_RESULT
        OUTPUT_QUIET
        ERROR_QUIET
    )
    
    if(CONAN_RESULT EQUAL 0)
        message(STATUS "Conan is installed, setting up dependencies")
        # Run conan install
        execute_process(
            COMMAND conan install ${CMAKE_SOURCE_DIR} -of ${CMAKE_BINARY_DIR} --build=missing
            RESULT_VARIABLE CONAN_INSTALL_RESULT
        )
        
        if(CONAN_INSTALL_RESULT EQUAL 0 AND EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
            include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
            conan_basic_setup()
            set(USE_CONAN TRUE)
            message(STATUS "Conan dependencies installed successfully")
        else()
            message(WARNING "Conan install failed, will try other methods")
        endif()
    else()
        message(STATUS "Conan not found, will try other methods")
    endif()
endif()

# Check for vcpkg if Conan is not used
if(NOT USE_CONAN)
    # Check if VCPKG_ROOT is defined or if vcpkg toolchain file exists in standard locations
    if(DEFINED ENV{VCPKG_ROOT})
        set(VCPKG_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(VCPKG_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    elseif(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(VCPKG_TOOLCHAIN_FILE "${CMAKE_CURRENT_BINARY_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    endif()
    
    if(DEFINED VCPKG_TOOLCHAIN_FILE AND EXISTS "${VCPKG_TOOLCHAIN_FILE}")
        message(STATUS "Using vcpkg for dependency management")
        include("${VCPKG_TOOLCHAIN_FILE}")
        set(USE_VCPKG TRUE)
        
        # Find packages with vcpkg
        find_package(SDL2 REQUIRED)
        find_package(SDL2_image REQUIRED)
        find_package(UUID REQUIRED)
        # Uncomment if needed
        # find_package(GTest REQUIRED)
    endif()
endif()

# Fall back to pkg-config if neither Conan nor vcpkg is used
if(NOT USE_CONAN AND NOT USE_VCPKG)
    message(STATUS "Using pkg-config for dependency management")
    set(USE_PKG_CONFIG TRUE)
    INCLUDE(FindPkgConfig)
    pkg_check_modules(SDL REQUIRED sdl2)
    pkg_check_modules(SDL_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(UUID REQUIRED uuid)
    # Uncomment if needed
    # pkg_check_modules(GTEST REQUIRED gtest)
    # pkg_check_modules(TCMALLOC REQUIRED libtcmalloc)
endif()



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
include_directories(${CMAKE_SOURCE_DIR})

# Add include directories based on the dependency management system
if(USE_CONAN)
    # Conan includes are handled by conan_basic_setup()
elseif(USE_VCPKG)
    # vcpkg includes are handled by find_package()
elseif(USE_PKG_CONFIG)
    include_directories(${SDL_INCLUDE_DIRS})
    include_directories(${SDL_IMAGE_INCLUDE_DIRS})
    include_directories(${UUID_INCLUDE_DIRS})
    # Uncomment if needed
    # include_directories(${TCMALLOC_INCLUDE_DIRS})
    # include_directories(${GTEST_INCLUDE_DIRS})
endif()

#options
#currently is not implemented yet
unset(logging_game CACHE)
option(logging_game "Create log file and write all to it" OFF)
if (logging_game)
    add_definitions("-DMAKE_LOG")
    message(STATUS "Logging is on")
    set(logging_game ON)
else()
    message(STATUS "Logging is off")
endif (logging_game)

#results
add_executable(ssdl ${mIncludes} ${mSources} ${mEntityIncludes} ${mEntitySources} ${mProcessorsIncludes} ${mProcessorsSources} Processors/Common/NpcProcessor.cpp Processors/Common/NpcProcessor.h)

#linking
if(USE_CONAN)
    # Link with Conan-provided libraries
    target_link_libraries(ssdl ${CONAN_LIBS})
elseif(USE_VCPKG)
    # Link with vcpkg-provided libraries
    target_link_libraries(ssdl 
        SDL2::SDL2 
        SDL2::SDL2main
        SDL2_image::SDL2_image
        UUID::UUID
        # Uncomment if needed
        # GTest::GTest
        # GTest::Main
    )
elseif(USE_PKG_CONFIG)
    # Link with pkg-config-provided libraries
    target_link_libraries(ssdl 
        ${SDL_LIBRARIES}
        ${SDL_IMAGE_LIBRARIES}
        ${UUID_LIBRARIES}
        # Uncomment if needed
        # ${TCMALLOC_LIBRARIES}
        # ${GTEST_LIBRARIES}
    )
endif()
