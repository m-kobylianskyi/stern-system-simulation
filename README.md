# Sternsystem-Simulation

A real-time 2D gravity simulation with procedurally 
generated star systems.

## Features
- Newtonian gravity simulation (F = G × m1 × m2 / r²)
- Procedural star system generation
- Orbital mechanics with first cosmic velocity
- Custom ECS architecture (Sparse Set)
- Zoom support

## Tech Stack
- C++17
- SFML 3.0.2
- CMake 4.0+

## Build & Run
git clone https://github.com/m-kobylianskyi/stern-system-simulation.git
cd stern-system-simulation
cmake -S source -B build
cmake --build build
cd build
./Star

## Screenshots
<img width="2560" height="1277" alt="image" src="https://github.com/user-attachments/assets/2fa03c7a-ff34-4183-9df0-93ace7ddb4ea" />

## Architecture
The project uses a custom Entity Component System (ECS) 
with Sparse Set implementation for efficient component 
storage and retrieval with O(1) access time.
