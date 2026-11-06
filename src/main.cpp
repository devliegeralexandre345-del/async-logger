#include "logger.h"
#include <iostream>
#include <thread>
#include <vector>

int main() {
    Logger logger("log.txt");

    // Simulate some logging from multiple threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&logger, i]() {
            for (int j = 0; j < 100; ++j) {
                logger.log("Thread " + std::to_string(i) + ": Message " + std::to_string(j));
                std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate work
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}