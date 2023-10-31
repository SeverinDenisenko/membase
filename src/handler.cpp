#include "handler.hpp"

#include <fmt/format.h>
#include <glog/logging.h>

std::string mb::Handler::operator()(const std::string& request) noexcept
try {
    std::string res;

    size_t it = request.find('\0');
    if (it != std::string::npos) {
        res = request.substr(0, it);
    } else {
        res = "ERROR";
    }

    return res;
} catch (const std::exception& e) {
    LOG(ERROR) << fmt::format("Exception in handler: {}", e.what());
    return "ERROR";
}
