#pragma once

#include <optional>
#include <string>

#include <boost/unordered/unordered_flat_map.hpp>

namespace mb {
class DB {
public:
    DB();

    std::optional<std::string> get(const std::string& key) noexcept;
    void put(const std::string& key, const std::string& value) noexcept;
    void remove(const std::string& key) noexcept;

private:
    boost::unordered_flat_map<std::string, std::string> map;
};
}
