project(DeepPic CXX C)

cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE HEADER_FILES project/include/*.h)
file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

include(InstallLibraries.cmake)

add_subdirectory(commands)
add_subdirectory(backend)

add_executable(main ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(main PRIVATE backend)

target_include_directories(main PRIVATE commands/project/include)
target_include_directories(main PRIVATE backend/project/include)