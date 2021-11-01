# beware dark magic!

include(FetchContent)

FetchContent_Declare(
        boost-cmake
        GIT_REPOSITORY https://github.com/ITesserakt/boost-cmake
)

FetchContent_Declare(
        ranges
        GIT_REPOSITORY https://github.com/ericniebler/range-v3
)

FetchContent_MakeAvailable(boost-cmake ranges)

link_libraries(Boost::boost)