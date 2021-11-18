# beware dark magic!

include(FetchContent)

FetchContent_Declare(
        boost-cmake
        GIT_REPOSITORY https://github.com/ITesserakt/boost-cmake
)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929
)

FetchContent_Declare(
        cppcoro
        GIT_REPOSITORY https://github.com/arthurzam/cppcoro
)

FetchContent_Declare(
        smallvec
        GIT_REPOSITORY https://github.com/ITesserakt/SmallVec
)

FetchContent_MakeAvailable(boost-cmake cppcoro smallvec googletest)