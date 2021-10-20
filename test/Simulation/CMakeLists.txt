target_sources(${PROJECT_NAME} PRIVATE FluidVisualization.cpp)

target_link_libraries(${PROJECT_NAME} fluid_simulation_OpenGL)
target_link_libraries(${PROJECT_NAME} fluid_simulation_Simulation)


