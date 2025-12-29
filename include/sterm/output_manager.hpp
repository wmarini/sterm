#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <string_view>
#include <filesystem>

namespace sterm {

class OutputManager {
public:
    static OutputManager& instance();

    OutputManager(const OutputManager&) = delete;
    OutputManager& operator=(const OutputManager&) = delete;

    void set_log_file(const std::filesystem::path& log_path);
    void print(std::string_view message);
    void print_output(std::string_view command, std::string_view output);
    void print_prompt();
    void print_separator();
    void log(std::string_view message);

private:
    OutputManager() = default;

    std::mutex output_mutex_;
    std::ofstream log_file_;
};

} // namespace sterm
