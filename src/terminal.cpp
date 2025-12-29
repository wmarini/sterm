#include "sterm/terminal.hpp"
#include "sterm/version.hpp"

#include <iostream>

namespace sterm {

Terminal::Terminal() = default;

bool Terminal::initialize() {
    // Load configuration
    if (!config_.load()) {
        OutputManager::instance().print("Warning: Could not load config file, using defaults.\n");
    }

    // Set up logging
    OutputManager::instance().set_log_file(config_.log_file());
    OutputManager::instance().log("Terminal initialized");

    return true;
}

std::string Terminal::read_input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

boost::cobalt::task<void> Terminal::process_command(const std::string& input) {
    auto result = co_await command_handler_.execute(input);

    if (!result.output.empty() || !result.success) {
        OutputManager::instance().print_output(input, result.output);
    }

    if (result.should_quit) {
        running_ = false;
    }
}

boost::cobalt::task<void> Terminal::run() {
    running_ = true;

    // Print welcome message
    OutputManager::instance().print(std::string(PROGRAM_NAME) + " v" + std::string(VERSION) + "\n");
    OutputManager::instance().print("Type 'help' for available commands, 'quit' to exit.\n\n");

    while (running_) {
        OutputManager::instance().print_prompt();

        std::string input = read_input();

        // Handle EOF (Ctrl+D)
        if (std::cin.eof()) {
            OutputManager::instance().print("\n");
            break;
        }

        // Skip empty input
        if (input.empty()) {
            continue;
        }

        co_await process_command(input);
    }

    OutputManager::instance().log("Terminal session ended");
}

} // namespace sterm
