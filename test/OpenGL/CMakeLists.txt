target_sources(${PROJECT_NAME} PRIVATE Window.cpp VertexAttributes.cpp ShaderProgram.cpp PixelMap.cpp ImGui.cpp)

target_link_libraries(${PROJECT_NAME} fluid_simulation_OpenGL)
target_link_libraries(${PROJECT_NAME} ImGui)
