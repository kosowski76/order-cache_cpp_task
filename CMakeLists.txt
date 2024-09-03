cmake_minimum_required(VERSION 3.10)
project(OrderCacheTest CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add source files
add_library(OrderCache src/OrderCache.cpp)

# Add test files
enable_testing()

# Find Google Test package
find_package(GTest REQUIRED)

add_executable(OrderCacheTest tests/OrderCacheTest.cpp)

# Link Google Test libraries and pthread
# target_link_libraries(OrderCacheTest GTest::GTest GTest::Main pthread)
target_link_libraries(OrderCacheTest PRIVATE OrderCache GTest::GTest GTest::Main)

# Add test
add_test(NAME OrderCacheTest COMMAND OrderCacheTest)
