project(Tests)

add_executable(${PROJECT_NAME})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} gtest gmock gtest_main)

add_subdirectory(OpenGL)
add_subdirectory(Simulation)

