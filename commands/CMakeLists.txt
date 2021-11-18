file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)
file(GLOB_RECURSE TEST_FILES project/test/*.cpp)

add_library(commands ${SOURCE_FILES})

target_include_directories(commands PRIVATE project/include)

add_executable(commands_test ${TEST_FILES})
target_include_directories(commands_test PRIVATE project/include)
target_link_libraries(commands_test PUBLIC commands gtest_main)