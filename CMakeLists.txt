project(DeepPic CXX C)

cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

include(InstallLibraries.cmake)

add_subdirectory(commands)
add_subdirectory(backend)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE commands pthread)

target_include_directories(main PRIVATE commands/project/include)
target_include_directories(main PRIVATE backend/project/include)
target_include_directories(main PRIVATE project/include)