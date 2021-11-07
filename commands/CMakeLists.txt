file(GLOB_RECURSE SOURCE_FILES project/src/*.cpp)

add_library(commands ${SOURCE_FILES})

target_include_directories(commands PRIVATE ../backend/project/include
        PRIVATE project/include)
target_link_libraries(commands backend)