#pragma once

#include <string>

namespace mb {
class Config {
public:
    Config(const std::string& file) noexcept;
    Config() = default;

    std::string host;
    int port;
    size_t memory;
    int max_request_length;
};
}
