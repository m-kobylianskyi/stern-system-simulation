cmake_minimum_required(VERSION 4.0)
project(StarSystemSimulation VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(Star main.cpp entity.cpp system.cpp scene.cpp)

target_compile_features(Star PRIVATE cxx_std_17)
target_link_libraries(Star PRIVATE SFML::Graphics)