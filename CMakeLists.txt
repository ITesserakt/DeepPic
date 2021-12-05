cmake_minimum_required(VERSION 3.20)
project(boost_asio_async_server)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.40.0 REQUIRED system)
find_package(Boost COMPONENTS log log_setup REQUIRED)
link_directories(${Boost_LIBRARY_DIR})

include_directories(project/include ${Boost_INCLUDE_DIR})
aux_source_directory(project/src SRC)
add_executable(boost_asio_async_server ${SRC} project/ServerConnection/include/ServerConnection.h project/ServerConnection/src/ServerConnection.cpp)

target_link_libraries(boost_asio_async_server boost_thread pthread ${BOOST_LIBRARIES})
