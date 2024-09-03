cmake_minimum_required(VERSION 3.10)
project(OrderCacheTest CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add source files
add_library(OrderCache src/OrderCache.cpp)

# Enable testing
enable_testing()

# Find Google Test package
# find_package(GTest REQUIRED)
# Use FetchContent to include Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# Fetch the content and make it available
FetchContent_MakeAvailable(googletest)

add_executable(OrderCacheTest tests/OrderCacheTest.cpp)

# Link Google Test libraries and pthread
target_link_libraries(OrderCacheTest PRIVATE OrderCache gtest_main)
# target_link_libraries(OrderCacheTest GTest::GTest GTest::Main pthread)
# target_link_libraries(OrderCacheTest PRIVATE OrderCache GTest::GTest GTest::Main)

# Add test
add_test(NAME OrderCacheTest COMMAND OrderCacheTest)
