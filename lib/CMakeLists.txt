#glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})


#OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})


#GLFW
find_package(glfw3 REQUIRED)

#ImGui
set(IMGUI_PATH "imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
file(GLOB IMGUI_HEADERS ${IMGUI_PATH}/*.h)

add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_PATH})

target_link_libraries(ImGui PRIVATE glfw)
target_link_libraries(ImGui PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(ImGui PRIVATE ${GLEW_LIBRARIES})


