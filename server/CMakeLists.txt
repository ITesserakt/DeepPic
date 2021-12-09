file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)
file(GLOB_RECURSE TEST_FILES project/test/*.cpp)

add_executable(server ${SOURCE_FILES})

target_include_directories(server PUBLIC project/include)
target_link_libraries(server PUBLIC commands)

add_executable(server_test ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(server_test PRIVATE project/include)
target_link_libraries(server_test PRIVATE gtest_main)
