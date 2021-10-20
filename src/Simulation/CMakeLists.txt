set(SIMULATION_SOURCES FluidBuilder.cpp Fluid.cpp Fluid.cpp FluidSimulation.cpp FluidVisualisation.cpp Presentation.cpp)

add_library(${PROJECT_NAME}_Simulation STATIC ${SIMULATION_SOURCES})
target_include_directories(${PROJECT_NAME}_Simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(${PROJECT_NAME}_Simulation PRIVATE ${PROJECT_NAME}_OpenGL)
target_link_libraries(${PROJECT_NAME}_Simulation PRIVATE ${PROJECT_NAME}_Utilities)