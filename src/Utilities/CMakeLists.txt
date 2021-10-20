set(UTILITY_SOURCES MouseInput.cpp)
add_library(${PROJECT_NAME}_Utilities STATIC ${UTILITY_SOURCES})
target_include_directories(${PROJECT_NAME}_Utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(${PROJECT_NAME}_Utilities PRIVATE ${PROJECT_NAME}_OpenGL)


