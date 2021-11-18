file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)
file(GLOB_RECURSE TEST_FILES project/test/*.cpp)

add_library(backend ${SOURCE_FILES})

target_include_directories(backend PRIVATE project/include)

add_executable(backend_test ${TEST_FILES})
target_include_directories(backend_test PRIVATE project/include)
target_link_libraries(backend_test PUBLIC backend gtest_main)