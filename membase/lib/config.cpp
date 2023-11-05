#include "config.hpp"

#include <fstream>

#include <fmt/format.h>
#include <glog/logging.h>
#include <nlohmann/json.hpp>

mb::Config::Config(const std::string& file) noexcept
{
    try {
        nlohmann::json json;
        std::fstream stream(file);
        if (!stream) {
            LOG(FATAL) << fmt::format("Can't open file {}", file);
        }
        stream >> json;

        mode = json["mode"];

        host = json["server"]["host"];
        port = json["server"]["port"];
        max_request_length = json["server"]["max_request_length"];

        memory = json["cache"]["memory"];

        path = json["persistent"]["path"];
        create_if_missing = json["persistent"]["create_if_missing"];
    } catch (const std::exception& e) {
        LOG(FATAL) << fmt::format("Can't load config! Reason: {}", e.what());
    }
}
