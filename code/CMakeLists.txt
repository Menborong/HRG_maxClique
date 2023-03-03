cmake_minimum_required(VERSION 3.0.0)
project(HRG_Clique VERSION 0.1.0)

set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(source_dir  "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
    ${source_dir}/reduction.cpp
    ${source_dir}/CNEEObuilder.cpp
    ${source_dir}/MaximalClique.cpp
    ${source_dir}/vertexCover.cpp
    ${source_dir}/MaximumClique.cpp
)

set(HEADERS
    ${include_dir}/Graph.hpp
    ${include_dir}/reduction.hpp
    ${include_dir}/CNEEObuilder.hpp
    ${include_dir}/MaximalClique.hpp
    ${include_dir}/vertexCover.hpp
    ${include_dir}/MaximumClique.hpp
)

add_library(HRG_Clique ${SOURCES} ${HEADERS})

add_subdirectory(test)