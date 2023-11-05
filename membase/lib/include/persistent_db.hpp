#pragma once

#include <shared_mutex>

#include "config.hpp"
#include "db.hpp"

#include <leveldb/db.h>

namespace mb {

class PersistentDB : public DB {
public:
    PersistentDB(Config& config);
    ~PersistentDB() override;

    std::optional<ValueType> get(const KeyType&& key) noexcept override;
    void put(const KeyType&& key, const ValueType&& value) noexcept override;
    void remove(const KeyType&& key) noexcept override;
    void wipe() noexcept override;
    std::unordered_set<KeyType> findKey(const KeyType&& key) noexcept override;
    std::unordered_set<KeyType> findValue(const ValueType&& value) noexcept override;

private:
    std::shared_mutex mutex;
    Config& config;
    leveldb::DB* db;
    leveldb::Options options;
    leveldb::WriteOptions write_options;
    leveldb::ReadOptions read_options;
};
}
