cmake_minimum_required(VERSION 3.10)
project(OrderCacheTest CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add source files
add_library(OrderCache src/OrderCache.cpp)

# Add Catch2 test files
enable_testing()
#find_package(Catch2 REQUIRED)
add_executable(test_OrderCache tests/OrderCacheTest.cpp)
#target_link_libraries(test_OrderCache PRIVATE OrderCache Catch2::Catch2)

# Add test
add_test(NAME OrderCacheTest COMMAND test_OrderCache)
