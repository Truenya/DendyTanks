cmake_minimum_required(VERSION 3.20)

# Special handling for Nix environments to fix compiler detection issues
if(DEFINED ENV{NIX_CC} OR DEFINED ENV{NIX_STORE} OR DEFINED ENV{IN_NIX_SHELL})
    message(STATUS "Nix environment detected, handling compiler detection")
    
    # Try multiple approaches to find working compilers in Nix
    
    # Approach 1: Use simple compiler names that resolve through PATH
    if(NOT DEFINED CMAKE_C_COMPILER OR NOT DEFINED CMAKE_CXX_COMPILER)
        message(STATUS "Trying simple compiler names...")
        set(ENV{CC} "cc")
        set(ENV{CXX} "c++")
        set(CMAKE_C_COMPILER "cc" CACHE FILEPATH "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER "c++" CACHE FILEPATH "C++ compiler" FORCE)
    endif()
    
    # Approach 2: Find actual executables in PATH
    if(NOT DEFINED CMAKE_C_COMPILER OR NOT DEFINED CMAKE_CXX_COMPILER)
        message(STATUS "Trying to find compiler executables in PATH...")
        execute_process(
            COMMAND which cc
            OUTPUT_VARIABLE CC_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND which c++
            OUTPUT_VARIABLE CXX_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        
        if(CC_PATH AND CXX_PATH)
            message(STATUS "Found C compiler: ${CC_PATH}")
            message(STATUS "Found C++ compiler: ${CXX_PATH}")
            set(CMAKE_C_COMPILER "${CC_PATH}" CACHE FILEPATH "C compiler" FORCE)
            set(CMAKE_CXX_COMPILER "${CXX_PATH}" CACHE FILEPATH "C++ compiler" FORCE)
        endif()
    endif()
    
    # Approach 3: Use gcc/g++ directly if cc/c++ didn't work
    if(NOT DEFINED CMAKE_C_COMPILER OR NOT DEFINED CMAKE_CXX_COMPILER)
        message(STATUS "Trying gcc/g++...")
        execute_process(
            COMMAND which gcc
            OUTPUT_VARIABLE GCC_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND which g++
            OUTPUT_VARIABLE GXX_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        
        if(GCC_PATH AND GXX_PATH)
            message(STATUS "Found GCC: ${GCC_PATH}")
            message(STATUS "Found G++: ${GXX_PATH}")
            set(CMAKE_C_COMPILER "${GCC_PATH}" CACHE FILEPATH "C compiler" FORCE)
            set(CMAKE_CXX_COMPILER "${GXX_PATH}" CACHE FILEPATH "C++ compiler" FORCE)
        endif()
    endif()
    
    # Final message about compiler settings
    message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
    message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")
    
    # Instruct users if we still don't have compilers
    if(NOT DEFINED CMAKE_C_COMPILER OR NOT DEFINED CMAKE_CXX_COMPILER)
        message(FATAL_ERROR "Could not find C/C++ compilers in Nix environment. Please run:\n"
                           "cmake -B build -DCMAKE_C_COMPILER=\$(which cc) -DCMAKE_CXX_COMPILER=\$(which c++)")
    endif()
endif()

# Set language before project to avoid compiler checks
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(ssdl LANGUAGES C CXX)

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
elseif(EXISTS ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    message(STATUS "Using Conan 2.x configuration")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    set(USE_CONAN TRUE)
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
        
        # Get Conan version to determine which approach to use
        execute_process(
            COMMAND ${CONAN_COMMAND} --version
            RESULT_VARIABLE CONAN_RESULT
            OUTPUT_VARIABLE CONAN_VERSION_OUTPUT
            ERROR_QUIET
        )
        
        if(CONAN_RESULT EQUAL 0)
            message(STATUS "Conan is installed: ${CONAN_VERSION_OUTPUT}")
            
            # Check if it's Conan 2.x
            if(CONAN_VERSION_OUTPUT MATCHES "Conan version 2")
                message(STATUS "Detected Conan 2.x, using modern generators")
                # Run conan install with Conan 2.x
                execute_process(
                    COMMAND ${CONAN_COMMAND} install ${CMAKE_SOURCE_DIR} 
                            --output-folder=${CMAKE_BINARY_DIR} 
                            --build=missing
                    RESULT_VARIABLE CONAN_INSTALL_RESULT
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                )
                
                if(CONAN_INSTALL_RESULT EQUAL 0 AND EXISTS ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
                    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
                    set(USE_CONAN TRUE)
                    message(STATUS "Conan 2.x dependencies installed successfully")
                else()
                    message(WARNING "Conan 2.x install failed, will try other methods")
                endif()
            else()
                # Run conan install with Conan 1.x
                message(STATUS "Detected Conan 1.x, using legacy generators")
                execute_process(
                    COMMAND ${CONAN_COMMAND} install ${CMAKE_SOURCE_DIR} --install-folder=${CMAKE_BINARY_DIR} --build=missing
                    RESULT_VARIABLE CONAN_INSTALL_RESULT
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                )
                
                if(CONAN_INSTALL_RESULT EQUAL 0 AND EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
                    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
                    conan_basic_setup()
                    set(USE_CONAN TRUE)
                    message(STATUS "Conan 1.x dependencies installed successfully")
                else()
                    message(WARNING "Conan 1.x install failed, will try other methods")
                endif()
            endif()
        else()
            message(STATUS "Conan not found or not working, will try other methods")
        endif()
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
    elseif(EXISTS "/usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(VCPKG_TOOLCHAIN_FILE "/usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
    endif()
    
    # If vcpkg toolchain file is found, use it
    if(DEFINED VCPKG_TOOLCHAIN_FILE AND EXISTS "${VCPKG_TOOLCHAIN_FILE}")
        message(STATUS "Using vcpkg for dependency management: ${VCPKG_TOOLCHAIN_FILE}")
        include("${VCPKG_TOOLCHAIN_FILE}")
        set(USE_VCPKG TRUE)
        
        # Find packages with vcpkg
        find_package(SDL2 REQUIRED)
        find_package(SDL2_image REQUIRED)
        find_package(UUID REQUIRED)
        # Uncomment if needed
        # find_package(GTest REQUIRED)
    else()
        # Try to find vcpkg executable and bootstrap it if needed
        find_program(VCPKG_EXECUTABLE vcpkg)
        
        if(VCPKG_EXECUTABLE)
            message(STATUS "Found vcpkg executable: ${VCPKG_EXECUTABLE}")
            
            # Get vcpkg root directory
            execute_process(
                COMMAND ${VCPKG_EXECUTABLE} root
                OUTPUT_VARIABLE VCPKG_ROOT
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            
            if(VCPKG_ROOT)
                set(VCPKG_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
                
                # Check if vcpkg.json exists and install dependencies
                if(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg.json")
                    message(STATUS "Found vcpkg.json, installing dependencies")
                    
                    # Install dependencies using vcpkg
                    execute_process(
                        COMMAND ${VCPKG_EXECUTABLE} install --triplet=x64-linux
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        RESULT_VARIABLE VCPKG_INSTALL_RESULT
                    )
                    
                    if(VCPKG_INSTALL_RESULT EQUAL 0)
                        message(STATUS "vcpkg dependencies installed successfully")
                    else()
                        message(WARNING "vcpkg install failed with code: ${VCPKG_INSTALL_RESULT}")
                    endif()
                endif()
                
                # Include vcpkg toolchain file
                if(EXISTS "${VCPKG_TOOLCHAIN_FILE}")
                    message(STATUS "Using vcpkg toolchain: ${VCPKG_TOOLCHAIN_FILE}")
                    include("${VCPKG_TOOLCHAIN_FILE}")
                    set(USE_VCPKG TRUE)
                    
                    # Find packages with vcpkg
                    find_package(SDL2 REQUIRED)
                    find_package(SDL2_image REQUIRED)
                    find_package(UUID REQUIRED)
                    # Uncomment if needed
                    # find_package(GTest REQUIRED)
                else()
                    message(STATUS "vcpkg toolchain file not found at ${VCPKG_TOOLCHAIN_FILE}")
                endif()
            else()
                message(STATUS "Could not determine vcpkg root directory")
            endif()
        else()
            message(STATUS "vcpkg executable not found")
        endif()
    endif()
endif()

# Fall back to pkg-config if neither Conan nor vcpkg is used
if(NOT USE_CONAN AND NOT USE_VCPKG)
    message(STATUS "Using pkg-config for dependency management")
    set(USE_PKG_CONFIG TRUE)
    
    # Check if pkg-config is available
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        message(STATUS "pkg-config found: ${PKG_CONFIG_EXECUTABLE}")
        
        # Try to find required packages
        pkg_check_modules(SDL2 REQUIRED sdl2)
        pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
        pkg_check_modules(UUID REQUIRED uuid)
        # Optional packages
        pkg_check_modules(GTEST gtest)
        
        # Check if all required dependencies were found
        if(SDL2_FOUND AND SDL2_IMAGE_FOUND AND UUID_FOUND)
            message(STATUS "All required dependencies found via pkg-config")
        else()
            message(FATAL_ERROR "Not all required dependencies were found via pkg-config")
        endif()
        
        # Report found package versions
        message(STATUS "SDL2 version: ${SDL2_VERSION}")
        message(STATUS "SDL2_image version: ${SDL2_IMAGE_VERSION}")
        message(STATUS "UUID version: ${UUID_VERSION}")
        if(GTEST_FOUND)
            message(STATUS "GTest version: ${GTEST_VERSION}")
        endif()
    else()
        message(FATAL_ERROR "pkg-config not found and neither Conan nor vcpkg are available. Cannot resolve dependencies.")
    endif()
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

# Always include the project source directory
include_directories(${CMAKE_SOURCE_DIR})

# Add include directories based on the dependency management system
if(USE_CONAN)
    # Conan includes are handled by conan_basic_setup() for Conan 1.x
    # For Conan 2.x, the toolchain file handles includes
    message(STATUS "Using Conan-provided include directories")
elseif(USE_VCPKG)
    # vcpkg includes are handled by find_package()
    message(STATUS "Using vcpkg-provided include directories")
elseif(USE_PKG_CONFIG)
    # Add pkg-config include directories
    include_directories(${SDL2_INCLUDE_DIRS})
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    include_directories(${UUID_INCLUDE_DIRS})
    
    if(GTEST_FOUND)
        include_directories(${GTEST_INCLUDE_DIRS})
    endif()
    
    message(STATUS "Using pkg-config include directories:")
    message(STATUS "  SDL2: ${SDL2_INCLUDE_DIRS}")
    message(STATUS "  SDL2_IMAGE: ${SDL2_IMAGE_INCLUDE_DIRS}")
    message(STATUS "  UUID: ${UUID_INCLUDE_DIRS}")
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
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${UUID_LIBRARIES}
        # Uncomment if needed
        # ${TCMALLOC_LIBRARIES}
        # ${GTEST_LIBRARIES}
    )
endif()

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)
