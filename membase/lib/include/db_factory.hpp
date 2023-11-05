#pragma once

#include <memory>

#include "config.hpp"
#include "db.hpp"

namespace mb {
class DBFactory {
public:
    explicit DBFactory(Config& config) noexcept;

    std::unique_ptr<DB> create() noexcept;

private:
    Config& config;
};
}
