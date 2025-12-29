#include "sterm/command_handler.hpp"
#include "sterm/version.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace sterm {

CommandHandler::CommandHandler() {
    // Register built-in commands
    register_command("help", [this](const CommandArgs& args) { return cmd_help(args); },
                     "Display available commands and their descriptions");

    register_command("version", [this](const CommandArgs& args) { return cmd_version(args); },
                     "Show program version information");

    register_command("date", [this](const CommandArgs& args) { return cmd_date(args); },
                     "Display current date and time");

    register_command("history", [this](const CommandArgs& args) { return cmd_history(args); },
                     "Show command history");

    register_command("quit", [this](const CommandArgs& args) { return cmd_quit(args); },
                     "Exit the program");
}

void CommandHandler::register_command(const std::string& name, CommandFunc func,
                                       const std::string& description) {
    commands_[name] = std::move(func);
    descriptions_[name] = description;
}

std::pair<std::string, CommandHandler::CommandArgs> CommandHandler::parse_input(
    const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    CommandArgs args;

    iss >> command;

    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }

    return {command, args};
}

void CommandHandler::add_to_history(const std::string& input) {
    if (input.empty()) {
        return;
    }

    history_.push_back(input);

    if (history_.size() > max_history_size_) {
        history_.pop_front();
    }
}

boost::cobalt::promise<CommandResult> CommandHandler::execute(const std::string& input) {
    auto [command, args] = parse_input(input);

    if (command.empty()) {
        co_return CommandResult{true, "", false};
    }

    add_to_history(input);

    auto it = commands_.find(command);
    if (it == commands_.end()) {
        co_return CommandResult{false, "Unknown command: " + command + "\nType 'help' for available commands.", false};
    }

    co_return co_await it->second(args);
}

boost::cobalt::promise<CommandResult> CommandHandler::cmd_help(const CommandArgs& args) {
    std::ostringstream oss;

    if (args.empty()) {
        oss << "Available commands:\n\n";
        for (const auto& [name, desc] : descriptions_) {
            oss << "  " << std::left << std::setw(12) << name << " - " << desc << '\n';
        }
        oss << "\nType 'help <command>' for more information about a specific command.";
    } else {
        const auto& cmd_name = args[0];
        auto it = descriptions_.find(cmd_name);
        if (it != descriptions_.end()) {
            oss << cmd_name << ": " << it->second;
        } else {
            oss << "Unknown command: " << cmd_name;
        }
    }

    co_return CommandResult{true, oss.str(), false};
}

boost::cobalt::promise<CommandResult> CommandHandler::cmd_version(const CommandArgs& /*args*/) {
    std::ostringstream oss;
    oss << PROGRAM_NAME << " version " << VERSION << '\n';
    oss << DESCRIPTION;
    co_return CommandResult{true, oss.str(), false};
}

boost::cobalt::promise<CommandResult> CommandHandler::cmd_date(const CommandArgs& /*args*/) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&time, &tm_buf);

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%A, %B %d, %Y %H:%M:%S %Z");

    co_return CommandResult{true, oss.str(), false};
}

boost::cobalt::promise<CommandResult> CommandHandler::cmd_history(const CommandArgs& /*args*/) {
    std::ostringstream oss;

    if (history_.empty()) {
        oss << "No commands in history.";
    } else {
        oss << "Command history:\n";
        size_t index = 1;
        for (const auto& cmd : history_) {
            oss << "  " << std::setw(4) << index++ << "  " << cmd << '\n';
        }
    }

    co_return CommandResult{true, oss.str(), false};
}

boost::cobalt::promise<CommandResult> CommandHandler::cmd_quit(const CommandArgs& /*args*/) {
    co_return CommandResult{true, "Goodbye!", true};
}

} // namespace sterm
