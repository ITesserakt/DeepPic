project(DeepPic CXX)

cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE HEADER_FILES project/include/*.h)
file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

add_subdirectory(project/client)

add_executable(main ${HEADER_FILES} ${SOURCE_FILES})