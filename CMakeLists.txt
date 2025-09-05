cmake_minimum_required(VERSION 3.14)
project(LimitOrderBookSystem)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(GTest REQUIRED)

# Include directories
include_directories(src)

# Source files
set(SOURCES
    src/OrderBookEngine.cpp
    src/PriceLevel.cpp
)

# Create library
add_library(orderbook ${SOURCES})

# Test executable
add_executable(tests
    tests/test_orderbook_engine.cpp
    tests/test_price_level.cpp
)

target_link_libraries(tests orderbook GTest::gtest GTest::gtest_main)

# Enable testing
enable_testing()
add_test(NAME OrderBookTests COMMAND tests)