# beware dark magic!

include(FetchContent)

FetchContent_Declare(
        boost-cmake
        GIT_REPOSITORY https://github.com/ITesserakt/boost-cmake
)

FetchContent_Declare(
        cppcoro
        GIT_REPOSITORY https://github.com/arthurzam/cppcoro
)

FetchContent_Declare(
        smallvec
        GIT_REPOSITORY https://github.com/ITesserakt/SmallVec
)

FetchContent_MakeAvailable(boost-cmake cppcoro smallvec)