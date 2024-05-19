cmake_minimum_required(VERSION 3.16)
project(gems)

set(CMAKE_CXX_STANDARD 14)

# Find SFML package
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(gems main.cpp)

# Link SFML libraries
target_link_libraries(gems sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
