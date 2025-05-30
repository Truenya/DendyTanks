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
add_executable(managed_vector_tests
    managed_vector_tests.cpp
)

add_executable(position_tests
    position_tests.cpp
    ${CMAKE_SOURCE_DIR}/Entities/GameObjects/Position.cpp
)

add_executable(gameobject_tests
    gameobject_tests.cpp
    ${CMAKE_SOURCE_DIR}/Entities/GameObjects/GameObject.cpp
    ${CMAKE_SOURCE_DIR}/Entities/GameObjects/Position.cpp
)

add_executable(gameworld_tests
    gameworld_tests.cpp
    ${CMAKE_SOURCE_DIR}/Entities/GameObjects/GameWorld.cpp
    ${CMAKE_SOURCE_DIR}/Entities/GameObjects/GameObject.cpp
    ${CMAKE_SOURCE_DIR}/Entities/GameObjects/Position.cpp
)

add_executable(resource_manager_tests
    ResourceManagerTest.cpp
)

add_executable(game_resource_tests
    GameResourceTest.cpp
)

# Link with GTest
if(USE_CONAN)
    target_link_libraries(managed_vector_tests ${CONAN_LIBS})
    target_link_libraries(position_tests ${CONAN_LIBS})
    target_link_libraries(gameobject_tests ${CONAN_LIBS})
    target_link_libraries(gameworld_tests ${CONAN_LIBS})
elseif(USE_VCPKG)
    target_link_libraries(managed_vector_tests
        GTest::GTest
        GTest::Main
    )
    target_link_libraries(position_tests
        GTest::GTest
        GTest::Main
    )
    target_link_libraries(gameobject_tests
        GTest::GTest
        GTest::Main
    )
    target_link_libraries(gameworld_tests
        GTest::GTest
        GTest::Main
        uuid
    )
elseif(USE_PKG_CONFIG)
    target_link_libraries(managed_vector_tests
        ${GTEST_LIBRARIES}
    )
    target_link_libraries(position_tests
        ${GTEST_LIBRARIES}
    )
    target_link_libraries(gameobject_tests
        ${GTEST_LIBRARIES}
    )
    target_link_libraries(gameworld_tests
        ${GTEST_LIBRARIES}
        uuid
    )
    target_link_libraries(resource_manager_tests
        ${GTEST_LIBRARIES}
    )
    target_link_libraries(game_resource_tests
        ${GTEST_LIBRARIES}
    )
else()
    # Default case for simple builds
    target_link_libraries(managed_vector_tests gtest gtest_main pthread)
    target_link_libraries(position_tests gtest gtest_main pthread)
    target_link_libraries(gameobject_tests gtest gtest_main pthread)
    target_link_libraries(gameworld_tests gtest gtest_main pthread uuid)
    target_link_libraries(resource_manager_tests gtest gtest_main pthread)
    target_link_libraries(game_resource_tests gtest gtest_main pthread)
endif()

# Add tests to CTest
add_test(NAME ManagedVectorTests COMMAND managed_vector_tests)
add_test(NAME PositionTests COMMAND position_tests)
add_test(NAME GameObjectTests COMMAND gameobject_tests)
add_test(NAME GameWorldTests COMMAND gameworld_tests)
add_test(NAME ResourceManagerTests COMMAND resource_manager_tests)
add_test(NAME GameResourceTests COMMAND game_resource_tests)