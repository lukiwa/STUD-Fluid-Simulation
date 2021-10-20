# Fluid simulation

## Operating system
- Ubuntu based Linux operating system (tested on Kubuntu 20.04) 

OR

- WSL1, with XClient installed (eg. XLaunch with disabled "native opengl" and enabled "Disable access control"). 

## Prerequisites:
- sudo apt-get update
- CMake - sudo apt install cmake
- Build essentials - sudo apt-get install build-essential
- GoogleTest - sudo apt-get install libgtest-dev

- OpenGL - sudo apt-get install mesa-utils
- GLFW3 - sudo apt-get install libglfw3-dev
- GLEW - sudo apt-get install glew-utils && sudo apt-get install glew-utils

## Building:
- Clone this repository
- cd STUD-Fluid-Simulation
- mkdir build && cd build

For release mode:
- cmake ..  -DCMAKE_BUILD_TYPE=Release 
- Optionally, if building in WSL1 environemnt add option -DWSL1=ON
- Optionally, for building tests, add option -DBUILD_TESTS=enable

For release mode:
- cmake ..  -DCMAKE_BUILD_TYPE=Release 
- Optionally, if building in WSL1 environemnt add option -DWSL1=ON
- Optionally, for building tests, add option -DBUILD_TESTS=enable
- make -j $(nproc)

## Running application
- cd src
- ./fluid_simulation

## Running Tests (if enabled):
- cd test
- ./Tests


## For opening in Clion on Windows (with WSL1):
- Clion must be installed and configured to work with WSL1 - https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-product.html
- Clone this repository
- Open project in Clion
- Click "Trust this project"
- Accept shared configurations - click "OK"
- Appplication and tests can now be built and run from the Clion
