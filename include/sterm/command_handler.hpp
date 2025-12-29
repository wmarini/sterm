#pragma once

#include <boost/cobalt.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>

namespace sterm {

struct CommandResult {
    bool success{true};
    std::string output;
    bool should_quit{false};
};

class CommandHandler {
public:
    using CommandArgs = std::vector<std::string>;
    using CommandFunc = std::function<boost::cobalt::promise<CommandResult>(const CommandArgs&)>;

    CommandHandler();

    void register_command(const std::string& name, CommandFunc func, const std::string& description);
    boost::cobalt::promise<CommandResult> execute(const std::string& input);

    [[nodiscard]] const std::deque<std::string>& history() const { return history_; }
    [[nodiscard]] const std::unordered_map<std::string, std::string>& command_descriptions() const {
        return descriptions_;
    }

private:
    std::unordered_map<std::string, CommandFunc> commands_;
    std::unordered_map<std::string, std::string> descriptions_;
    std::deque<std::string> history_;
    static constexpr size_t max_history_size_ = 100;

    static std::pair<std::string, CommandArgs> parse_input(const std::string& input);
    void add_to_history(const std::string& input);

    // Built-in command implementations
    boost::cobalt::promise<CommandResult> cmd_help(const CommandArgs& args);
    boost::cobalt::promise<CommandResult> cmd_version(const CommandArgs& args);
    boost::cobalt::promise<CommandResult> cmd_date(const CommandArgs& args);
    boost::cobalt::promise<CommandResult> cmd_history(const CommandArgs& args);
    boost::cobalt::promise<CommandResult> cmd_quit(const CommandArgs& args);
};

} // namespace sterm
