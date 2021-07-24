# STUD-Fluid-Simulation
To build and run:
- git clone https://github.com/lukiwa/STUD-Fluid-Simulation.git
- cd STUD-Fluid-Simulation
- mkdir build && cd build
- cmake -DCMAKE_BUILD_TYPE=Debug or -DCMAKE_BUILD_TYPE=Release
- Optionally, if building in WSL1 environemnt add option -DWSLS1=ON
- make
- ./fluid_simulation
