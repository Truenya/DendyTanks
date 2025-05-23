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

# Add test executable
add_executable(managed_vector_tests
    managed_vector_tests.cpp
)

# Link with GTest
if(USE_CONAN)
    target_link_libraries(managed_vector_tests ${CONAN_LIBS})
elseif(USE_VCPKG)
    target_link_libraries(managed_vector_tests
        GTest::GTest
        GTest::Main
    )
elseif(USE_PKG_CONFIG)
    target_link_libraries(managed_vector_tests
        ${GTEST_LIBRARIES}
    )
else()
    # Default case for simple builds
    target_link_libraries(managed_vector_tests gtest gtest_main pthread)
endif()

# Add test to CTest
add_test(NAME ManagedVectorTests COMMAND managed_vector_tests)