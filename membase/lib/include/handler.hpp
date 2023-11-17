#pragma once

#include "db.hpp"
#include "string.hpp"

namespace mb {
class Handler {
public:
    explicit Handler(DB& db) noexcept;

    std::string operator()(const String& requst) noexcept;

private:
    DB& db;
};
}
