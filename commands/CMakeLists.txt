file(GLOB_RECURSE HEADER_FILES project/include/*.h)
file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

add_library(commands ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(commands INTERFACE ../backend/project/include
        PRIVATE project/include)
target_link_libraries(commands backend Boost::boost)