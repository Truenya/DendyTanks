cmake_minimum_required(VERSION 3.20)

# Enable testing
enable_testing()

# Find GTest package
if(USE_CONAN)
    # Conan should handle GTest dependencies
elseif(USE_VCPKG)
    find_package(GTest REQUIRED)
elseif(USE_PKG_CONFIG)
    pkg_check_modules(GTEST REQUIRED gtest)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Add test executables
add_executable(utility_tests
    utility_tests.cpp
)

add_executable(simple_tests
    simple_tests.cpp
)

# Link with GTest
if(USE_CONAN)
    target_link_libraries(utility_tests ${CONAN_LIBS})
    target_link_libraries(simple_tests ${CONAN_LIBS})
elseif(USE_VCPKG)
    target_link_libraries(utility_tests
        GTest::GTest
        GTest::Main
    )
    target_link_libraries(simple_tests
        GTest::GTest
        GTest::Main
    )
elseif(USE_PKG_CONFIG)
    target_link_libraries(utility_tests
        ${GTEST_LIBRARIES}
    )
    target_link_libraries(simple_tests
        ${GTEST_LIBRARIES}
    )
else()
    # Default case for simple builds
    target_link_libraries(utility_tests gtest gtest_main pthread)
    target_link_libraries(simple_tests gtest gtest_main pthread)
endif()

# Add tests to CTest
add_test(NAME UtilityTests COMMAND utility_tests)
add_test(NAME SimpleTests COMMAND simple_tests)