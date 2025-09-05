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
    tests/test_spsc_ring.cpp
    tests/test_wire.cpp
    tests/test_feeder.cpp
    tests/test_engine_runner.cpp
    tests/test_integration.cpp
)

target_link_libraries(tests orderbook GTest::gtest GTest::gtest_main)

# Enable testing
enable_testing()
add_test(NAME OrderBookTests COMMAND tests)