#include "logger.h"
#include <iomanip>
#include <sstream>

Logger::Logger(const std::string& filename)
    : filename_(filename) {
    file_stream_.open(filename_, std::ios::app);
    if (!file_stream_) {
        throw std::runtime_error("Failed to open log file");
    }
    writer_thread_ = std::thread(&Logger::worker_thread, this);
}

Logger::~Logger() {
    stop_.store(true);
    if (writer_thread_.joinable()) {
        writer_thread_.join();
    }
}

void Logger::log(std::string_view message) {
    size_t current_buffer_idx = current_buffer_idx_;
    Buffer& buffer = buffers_[current_buffer_idx];

    // Try to acquire a lock on the buffer
    if (buffer.mtx.try_lock()) {
        LogEntry entry{
            .timestamp = std::chrono::high_resolution_clock::now(),
            .msg = message,
        };

        size_t pos = buffer.write_pos.load();
        char* output = &buffer.data[pos];
        size_t written_size = format_log_entry(entry, output);
        
        if (pos + written_size < BUFFER_SIZE) {
            // Update position atomically
            buffer.write_pos += written_size;
            buffer.mtx.unlock();
        } else {
            // Buffer full - mark as full and switch buffers
            buffer.full.store(true);
            buffer.mtx.unlock();
            current_buffer_idx_ = (current_buffer_idx + 1) % 2; // Switch to next buffer
        }
    }
}

void Logger::worker_thread() {
    while (!stop_.load()) {
        for (size_t i = 0; i < 2; ++i) {
            Buffer& buf = buffers_[i];
            if (buf.full.load()) {
                flush_buffer(buf);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Avoid busy waiting
    }
}

bool Logger::flush_buffer(Buffer& buffer) {
    size_t pos = buffer.write_pos.load();
    if (pos == 0)
        return false;

    std::lock_guard<std::mutex> lock(buffer.mtx);
    file_stream_.write(buffer.data, pos);
    file_stream_.flush(); // Ensure data is written to disk
    
    buffer.write_pos = 0;
    buffer.full.store(false);
    return true;
}

size_t Logger::format_log_entry(const LogEntry& entry, char* output) {
    auto time_point = std::chrono::duration_cast<std::chrono::milliseconds>(
        entry.timestamp.time_since_epoch()).count();
    
    size_t written_size = 0;
    // Format timestamp
    written_size += snprintf(output + written_size, BUFFER_SIZE - written_size,
                             "[%.6d] %.*s\n", time_point, static_cast<int>(entry.msg.size()), entry.msg.data());
    return written_size;
}