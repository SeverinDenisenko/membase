#pragma once

#include "config.hpp"
#include "db.hpp"

#include <boost/unordered/unordered_flat_map.hpp>
#include <shared_mutex>

namespace mb {

class MemoryDB : public DB {
public:
    MemoryDB(Config& config);

    std::optional<ValueType> get(const KeyType&& key) noexcept override;
    void put(const KeyType&& key, const ValueType&& value) noexcept override;
    void remove(const KeyType&& key) noexcept override;
    void wipe() noexcept override;
    std::unordered_set<KeyType> findKey(const KeyType&& key) noexcept override;
    std::unordered_set<KeyType> findValue(const ValueType&& value) noexcept override;

private:
    std::shared_mutex mutex;
    boost::unordered_flat_map<KeyType, ValueType> map;
    Config& config;
};
}
