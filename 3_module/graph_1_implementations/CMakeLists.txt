cmake_minimum_required(VERSION 3.0)
project(algro_graph_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(algro_graph_1 main.cpp ListGraph.cpp ListGraph.h Graph.h MatrixGraph.cpp MatrixGraph.h SetGraph.cpp SetGraph.h ArcGraph.cpp ArcGraph.h)