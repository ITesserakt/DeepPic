file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

add_library(sha256 SHARED project/src/SHA256.c project/include/SHA256.h)
add_library(backend ${SOURCE_FILES})

target_link_libraries(backend INTERFACE sha256
        PUBLIC Boost::serialization)
target_include_directories(backend PUBLIC project/include)
target_include_directories(sha256 PUBLIC project/include)