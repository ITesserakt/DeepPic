file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)
file(GLOB_RECURSE TEST_FILES project/test/*.cpp)

add_library(client ${SOURCE_FILES})

target_include_directories(client PRIVATE project/include)

add_executable(client_test ${TEST_FILES})
target_include_directories(client_test PRIVATE project/include)
target_link_libraries(client_test PUBLIC client gtest_main)