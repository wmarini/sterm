#include "sterm/terminal.hpp"
#include "sterm/version.hpp"

#include <boost/cobalt.hpp>
#include <iostream>

int main() {
    try {
        sterm::Terminal terminal;

        if (!terminal.initialize()) {
            std::cerr << "Failed to initialize terminal\n";
            return 1;
        }

        boost::cobalt::run(terminal.run());

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}
