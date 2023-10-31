#pragma once

#include <string>

#include "db.hpp"

namespace mb {
class Handler {
public:
    explicit Handler(DB& db) noexcept;

    std::string operator()(const std::string& requst) noexcept;

private:
    DB& db;
};
}
