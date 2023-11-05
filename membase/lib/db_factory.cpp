#include "db_factory.hpp"

#include <glog/logging.h>
#include <fmt/format.h>

#include "memory_db.hpp"
#include "persistent_db.hpp"

mb::DBFactory::DBFactory(Config& config) noexcept
    : config(config)
{
}

std::unique_ptr<mb::DB> mb::DBFactory::create() noexcept{
    std::unique_ptr<mb::DB> result;

    if (config.mode == "memory") {
        result = std::make_unique<mb::MemoryDB>(config);
    } else if (config.mode == "persistent") {
        result = std::make_unique<mb::PersistentDB>(config);
    } else {
        LOG(FATAL) << fmt::format("Wrong mode: {}", config.mode);
    }

    return result;
}

