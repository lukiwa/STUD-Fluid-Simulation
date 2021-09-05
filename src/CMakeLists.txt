project(fluid_simulation)

set(ROOT_SOURCES main.cpp)
add_executable(${PROJECT_NAME} ${ROOT_SOURCES})

add_subdirectory(OpenGL)
add_subdirectory(Simulation)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_OpenGL)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_Simulation)
