cmake_minimum_required(VERSION 3.15)

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

enable_testing()
include(InstallLibraries.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(backend)
add_subdirectory(commands)
add_subdirectory(client)