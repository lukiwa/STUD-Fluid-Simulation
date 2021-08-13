cmake_minimum_required(VERSION 3.1)
project(fluid_simulation)

#debug / release
if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(">CMake in Debug mode")
    add_compile_definitions(DEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message(">CMake in Release mode")
endif ()

#allowing wsl display
if (WSL1)
    message(">Allowing WSL1 display by setting env variable")
    add_compile_definitions(WSL1)
else ()
    message(">NOT in WSL1")
endif ()

#compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_STANDARD 17)


#Project target
set(ROOT_SOURCES src/main.cpp)
set(OPENGL_SOURCES src/OpenGL/ShaderProgram.cpp src/OpenGL/VertexBuffer.cpp src/OpenGL/IndexBuffer.cpp src/OpenGL/VertexAttributes.cpp  src/OpenGL/VertexArray.cpp src/OpenGL/Texture.cpp src/OpenGL/Window.cpp src/OpenGL/PixelBufferObject.cpp src/OpenGL/PixelBufferObject.cpp)
#set(SIMULATION_SOURCES src/Simulation/*.cpp)
add_executable(${PROJECT_NAME} ${ROOT_SOURCES} ${OPENGL_SOURCES} ${SIMULATION_SOURCES})


#glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

#OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})


#GLFW
set(GLFW_INCLUDE_DIR "lib/glfw/include/GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
include_directories(GLFW_INCLUDE_DIR)
add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

#ImGui
set(IMGUI_PATH "lib/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
file(GLOB IMGUI_HEADERS ${IMGUI_PATH}/*.h)
add_library(ImGui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
include_directories(${IMGUI_PATH})
target_link_libraries(ImGui glfw)
target_link_libraries(ImGui ${OPENGL_LIBRARIES})
target_link_libraries(ImGui ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ImGui)










