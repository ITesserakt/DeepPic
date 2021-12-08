file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES project/include/*.h)
file(GLOB_RECURSE TEST_FILES project/test/*.cpp)

add_executable(client ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(client PUBLIC project/include ../commands/project/include)
target_link_libraries(client PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets commands)

add_executable(client_test ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(client_test PRIVATE project/include)
target_link_libraries(client_test PRIVATE gtest_main)
