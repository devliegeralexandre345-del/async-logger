#pragma once
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>
#include <vector>

class Logger {
public:
    explicit Logger(const std::string& filename);
    ~Logger();

    void log(std::string_view message);

private:
    struct LogEntry {
        std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
        std::string_view msg;
    };

    static constexpr size_t BUFFER_SIZE = 1024 * 1024; // 1MB
    static constexpr size_t MAX_LOGS_PER_BUFFER = 1000;

    struct Buffer {
        char data[BUFFER_SIZE];
        std::atomic<size_t> write_pos{0};
        std::atomic<bool> full{false};
        std::mutex mtx{};
    };

    void worker_thread();
    bool flush_buffer(Buffer& buffer);
    size_t format_log_entry(const LogEntry& entry, char* output);

    std::string filename_;
    std::atomic<bool> stop_{false};
    std::thread writer_thread_{};
    Buffer buffers_[2]; // Double buffering
    std::atomic<size_t> current_buffer_idx_{0};
    std::ofstream file_stream_{};
};