cmake_minimum_required(VERSION 3.16)
project(async_logger LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_executable(logger_app src/main.cpp src/logger.cpp)

enable_testing()
add_executable(unit_tests tests/test.cpp src/logger.cpp)
add_test(NAME AllTests COMMAND unit_tests)