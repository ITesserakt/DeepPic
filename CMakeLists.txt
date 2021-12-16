cmake_minimum_required(VERSION 3.20)
project(boost_asio_async_server)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.40.0 REQUIRED system)
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS log log_setup REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
link_directories(${Boost_LIBRARY_DIR})

include_directories(project/include ${Boost_INCLUDE_DIRS})
aux_source_directory(project/src SRCR)
add_executable(boost_asio_async_server ${SRCR})

target_link_libraries(boost_asio_async_server boost_thread pthread ${BOOST_LIBRARIES} nlohmann_json::nlohmann_json Boost::log Boost::log_setup)
