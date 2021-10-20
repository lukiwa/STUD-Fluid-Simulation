project(fluid_simulation)

set(ROOT_SOURCES main.cpp)
add_executable(${PROJECT_NAME} ${ROOT_SOURCES})

add_subdirectory(Utilities)
add_subdirectory(OpenGL)
add_subdirectory(Simulation)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_Utilities)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_OpenGL)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_Simulation)
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui)

