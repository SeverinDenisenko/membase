#include "config.hpp"

#include <fstream>

#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

mb::Config::Config(const std::string& file) noexcept{
    try {
        nlohmann::json json;
        std::fstream stream(file);
        stream >> json;

        host = json["host"];
        port = json["port"];
        max_request_length = json["max_request_length"];
        memory = json["memory"];
    } catch (const std::exception& e) {
        LOG(FATAL) << fmt::format("Can't load config! Reason: {}", e.what());
    }
}

