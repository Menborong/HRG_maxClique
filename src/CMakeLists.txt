cmake_minimum_required(VERSION 3.0.0)
project(maxCliqueTest VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(maxCliqueTest computeMaxCliqueTest.cpp CNEEObuilder.cpp Maximumclique.cpp reduction.cpp vertexCover.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
