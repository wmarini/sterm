#include "sterm/config.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>

namespace sterm {

Config::Config() : config_path_(default_config_path()) {}

std::filesystem::path Config::default_config_path() {
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        return {};
    }
    return std::filesystem::path(home) / ".config" / "sterm" / "config";
}

bool Config::load() {
    return load(config_path_);
}

bool Config::load(const std::filesystem::path& path) {
    config_path_ = path;

    if (!std::filesystem::exists(path)) {
        // Create default config directory and file
        auto config_dir = path.parent_path();
        if (!config_dir.empty()) {
            std::filesystem::create_directories(config_dir);
        }

        // Create default config file
        std::ofstream ofs(path);
        if (ofs) {
            ofs << "# sterm configuration file\n";
            ofs << "log_file=" << config_dir.string() << "/sterm.log\n";
            ofs.close();
        }
    }

    return parse_config_file(path);
}

bool Config::parse_config_file(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Find the '=' separator
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Trim whitespace
        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t\r\n"));
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
        };

        trim(key);
        trim(value);

        if (!key.empty()) {
            settings_[key] = value;
        }
    }

    return true;
}

std::filesystem::path Config::config_path() const {
    return config_path_;
}

std::filesystem::path Config::log_file() const {
    auto it = settings_.find("log_file");
    if (it != settings_.end()) {
        return it->second;
    }
    return config_path_.parent_path() / "sterm.log";
}

std::optional<std::string> Config::get(const std::string& key) const {
    auto it = settings_.find(key);
    if (it != settings_.end()) {
        return it->second;
    }
    return std::nullopt;
}

} // namespace sterm
