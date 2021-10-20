cmake_minimum_required(VERSION 3.16)
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
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#libs
add_subdirectory(lib)

#Project target
add_subdirectory(src)

#Tests
if (BUILD_TESTS)
    message(">Building tests")
    add_subdirectory(test)
endif ()









