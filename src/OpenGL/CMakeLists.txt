set(OPENGL_SOURCES ShaderProgram.cpp VertexBuffer.cpp IndexBuffer.cpp VertexAttributes.cpp VertexArray.cpp Texture.cpp Window.cpp PixelBufferObject.cpp PixelBufferObject.cpp PixelMap.cpp Renderer.cpp)

add_library(${PROJECT_NAME}_OpenGL STATIC  ${OPENGL_SOURCES})
target_include_directories(${PROJECT_NAME}_OpenGL PUBLIC ../Utilities)

target_link_libraries(${PROJECT_NAME}_OpenGL ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_OpenGL ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_OpenGL glfw)
target_link_libraries(${PROJECT_NAME}_OpenGL ImGui)