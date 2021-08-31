set(SIMULATION_SOURCES DyeMap.cpp FluidBuilder.cpp Fluid.cpp)

add_library(${PROJECT_NAME}_Simulation STATIC  ${SIMULATION_SOURCES})
target_include_directories(${PROJECT_NAME}_Simulation PUBLIC ../Utilities ../OpenGL)

target_link_libraries(${PROJECT_NAME}_Simulation ${PROJECT_NAME}_OpenGL)
