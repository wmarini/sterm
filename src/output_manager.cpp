#include "sterm/output_manager.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace sterm {

OutputManager& OutputManager::instance() {
    static OutputManager instance;
    return instance;
}

void OutputManager::set_log_file(const std::filesystem::path& log_path) {
    std::lock_guard<std::mutex> lock(output_mutex_);

    if (log_file_.is_open()) {
        log_file_.close();
    }

    // Create parent directories if they don't exist
    auto parent = log_path.parent_path();
    if (!parent.empty() && !std::filesystem::exists(parent)) {
        std::filesystem::create_directories(parent);
    }

    log_file_.open(log_path, std::ios::app);
}

void OutputManager::print(std::string_view message) {
    std::lock_guard<std::mutex> lock(output_mutex_);
    std::cout << message << std::flush;
}

void OutputManager::print_output(std::string_view command, std::string_view output) {
    std::lock_guard<std::mutex> lock(output_mutex_);

    // Print separator
    std::cout << "---\n";

    // Print output
    if (!output.empty()) {
        std::cout << output;
        if (output.back() != '\n') {
            std::cout << '\n';
        }
    }

    // Print closing separator
    std::cout << "---\n";

    // Log the command and output
    if (log_file_.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buf{};
        localtime_r(&time, &tm_buf);

        log_file_ << '[' << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "] "
                  << "Command: " << command << '\n';
        if (!output.empty()) {
            log_file_ << "Output:\n" << output << '\n';
        }
        log_file_ << "---\n" << std::flush;
    }
}

void OutputManager::print_prompt() {
    std::lock_guard<std::mutex> lock(output_mutex_);
    std::cout << "prompt> " << std::flush;
}

void OutputManager::print_separator() {
    std::lock_guard<std::mutex> lock(output_mutex_);
    std::cout << "---\n" << std::flush;
}

void OutputManager::log(std::string_view message) {
    std::lock_guard<std::mutex> lock(output_mutex_);

    if (log_file_.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buf{};
        localtime_r(&time, &tm_buf);

        log_file_ << '[' << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "] "
                  << message << '\n' << std::flush;
    }
}

} // namespace sterm
