project(Tests)

enable_testing()
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
target_link_libraries(${PROJECT_NAME} fluid_simulation_Utilities)


add_subdirectory(OpenGL)
add_subdirectory(Simulation)

