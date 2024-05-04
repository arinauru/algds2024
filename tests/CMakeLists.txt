include(FetchContent)

set(CMAKE_CXX_FLAGS "-O3")

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_executable(SplayTreeTests splay_tree_tests.cpp)
target_link_libraries(SplayTreeTests PRIVATE GTest::gtest_main)
target_include_directories(SplayTreeTests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(SplayTreeTests)
