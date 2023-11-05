#pragma once

#include <string>

namespace mb {
class Config {
public:
    Config(const std::string& file) noexcept;
    Config() = default;

    std::string mode;
    std::string host;
    int port;
    int max_request_length;
    size_t memory;
    std::string path;
    bool create_if_missing;
};
}
