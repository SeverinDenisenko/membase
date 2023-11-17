#pragma once

#include <boost/unordered/unordered_flat_map.hpp>
#include <shared_mutex>

#include "config.hpp"
#include "db.hpp"
#include "string.hpp"
#include "tracking_allocator.hpp"
#include "types.hpp"

namespace mb {
class MemoryDB : public DB {
public:
    MemoryDB(Config& config);

    ReturnValueResult get(const KeyType&& key) noexcept override;
    Status put(const KeyType&& key, const ValueType&& value) noexcept override;
    Status remove(const KeyType&& key) noexcept override;
    Status wipe() noexcept override;
    FindResult findKey(const KeyType&& key) noexcept override;
    FindResult findValue(const ValueType&& value) noexcept override;

private:
    Config& config_;
    std::shared_mutex mutex_;
    boost::unordered_flat_map<InternalKeyType, InternalValueType, boost::hash<InternalKeyType>> map_;
};
}
