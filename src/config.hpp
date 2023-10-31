#pragma once

#include <string>

namespace mb {
struct Config {
    std::string host;
    int port;
    size_t memory;
    int max_request_length;
};
}
