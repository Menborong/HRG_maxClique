cmake_minimum_required(VERSION 3.0.0)
project(HRG_Clique VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -W -O2)

set(include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(source_dir  "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
    ${source_dir}/Graph.cpp
    ${source_dir}/Reduction.cpp
    ${source_dir}/CNEEObuilder.cpp
    ${source_dir}/MaximalClique.cpp
    ${source_dir}/BipartiteIndep.cpp
    ${source_dir}/MaximumClique.cpp
)

set(HEADERS
    ${include_dir}/Graph.hpp
    ${include_dir}/Reduction.hpp
    ${include_dir}/CNEEObuilder.hpp
    ${include_dir}/MaximalClique.hpp
    ${include_dir}/BipartiteIndep.hpp
    ${include_dir}/MaximumClique.hpp
)

add_library(HRG_Clique ${SOURCES} ${HEADERS})

target_include_directories(HRG_Clique PUBLIC ${include_dir})

# target_compile_features(HRG_Clique PUBLIC cxx_std_17)

# add_executable(reduction_test ${CMAKE_CURRENT_SOURCE_DIR}/test/reduction_test.cpp ${SOURCES} ${HEADERS})

add_executable(reduction_test test/reduction_test.cpp)
target_link_libraries(reduction_test HRG_Clique)

add_executable(solve_geo_test test/solve_geo_test.cpp)
target_link_libraries(solve_geo_test HRG_Clique)

add_executable(solve_nongeo_test test/solve_nongeo_test.cpp)
target_link_libraries(solve_nongeo_test HRG_Clique)