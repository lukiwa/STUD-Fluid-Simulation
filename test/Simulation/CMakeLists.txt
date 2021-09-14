target_sources(${PROJECT_NAME} PRIVATE FluidVisualization.cpp)
target_link_libraries(${PROJECT_NAME} fluid_simulation_OpenGL)
target_link_libraries(${PROJECT_NAME} fluid_simulation_Simulation)

target_include_directories(${PROJECT_NAME} PRIVATE ../../src/OpenGL/)
target_include_directories(${PROJECT_NAME} PRIVATE ../../src/Simulation/)

