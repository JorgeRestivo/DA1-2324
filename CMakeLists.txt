cmake_minimum_required(VERSION 3.26)
project(okda)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(okda
        Graph.cpp
        Graph.h
        main.cpp
        VertexEdge.cpp
        VertexEdge.h
        Reservoir.cpp
        Reservoir.h
        City.cpp
        City.h
        PumpingStation.cpp
        PumpingStation.h)
