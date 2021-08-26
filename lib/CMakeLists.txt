#glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})


#OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})


#GLFW
set(GLFW_INCLUDE_DIR "lib/glfw/include/GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
include_directories(GLFW_INCLUDE_DIR)
add_subdirectory(glfw)


#ImGui
set(IMGUI_PATH "imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
file(GLOB IMGUI_HEADERS ${IMGUI_PATH}/*.h)
add_library(ImGui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
include_directories(${IMGUI_PATH})
target_link_libraries(ImGui glfw)
target_link_libraries(ImGui ${OPENGL_LIBRARIES})
target_link_libraries(ImGui ${GLEW_LIBRARIES})

#Gtest
add_subdirectory(googletest)


