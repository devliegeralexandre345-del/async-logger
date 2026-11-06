#include <cassert>
#include <iostream>
#include "logger.h"

void test_basic_logging() {
    Logger logger("test_log.txt");
    logger.log("Test message 1");
    logger.log("Test message 2");
    std::cout << "Basic logging test passed\n";
}

int main() {
    try {
        test_basic_logging();
        std::cout << "All tests passed!\n";
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}