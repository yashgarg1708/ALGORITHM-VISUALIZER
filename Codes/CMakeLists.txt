cmake_minimum_required(VERSION 3.16)
project(AlgorithmVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(algoviz
    main.cpp
    display.cpp
    display_graph.cpp
    sorting.cpp
    graph.cpp
)

target_link_libraries(algoviz PRIVATE SFML::Graphics SFML::Window SFML::System)
target_compile_options(algoviz PRIVATE -Wall -Wextra -Wpedantic)
