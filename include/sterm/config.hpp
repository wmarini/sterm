#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

namespace sterm {

class Config {
public:
    Config();

    bool load();
    bool load(const std::filesystem::path& path);

    [[nodiscard]] std::filesystem::path config_path() const;
    [[nodiscard]] std::filesystem::path log_file() const;
    [[nodiscard]] std::optional<std::string> get(const std::string& key) const;

private:
    std::filesystem::path config_path_;
    std::unordered_map<std::string, std::string> settings_;

    bool parse_config_file(const std::filesystem::path& path);
    static std::filesystem::path default_config_path();
};

} // namespace sterm
