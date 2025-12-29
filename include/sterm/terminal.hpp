#pragma once

#include "sterm/command_handler.hpp"
#include "sterm/config.hpp"
#include "sterm/output_manager.hpp"

#include <boost/cobalt.hpp>
#include <string>

namespace sterm {

class Terminal {
public:
    Terminal();

    bool initialize();
    boost::cobalt::task<void> run();

private:
    Config config_;
    CommandHandler command_handler_;
    bool running_{false};

    std::string read_input();
    boost::cobalt::task<void> process_command(const std::string& input);
};

} // namespace sterm
