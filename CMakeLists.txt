cmake_minimum_required(VERSION 3.19)

project(DeepPic VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT WIN32)
  set(CPPCORO_USE_IO_RING ON)
  add_compile_definitions(CPPCORO_USE_IO_RING)
else ()
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

include(InstallLibraries.cmake)

add_subdirectory(commands)
add_subdirectory(backend)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE commands)
if (NOT WIN32)
  target_link_libraries(main PRIVATE pthread)
endif ()

target_include_directories(main PRIVATE commands/project/include)
target_include_directories(main PRIVATE backend/project/include)
target_include_directories(main PRIVATE project/include)