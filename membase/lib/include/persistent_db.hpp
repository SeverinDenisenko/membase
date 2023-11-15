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

    Result<ValueType> get(const KeyType&& key) noexcept override;
    Status put(const KeyType&& key, const ValueType&& value) noexcept override;
    Status remove(const KeyType&& key) noexcept override;
    Status wipe() noexcept override;
    FindResult findKey(const KeyType&& key) noexcept override;
    FindResult findValue(const ValueType&& value) noexcept override;

private:
    std::shared_mutex mutex_;
    Config& config_;
    leveldb::DB* db_;
    leveldb::Options options_;
    leveldb::WriteOptions write_options_;
    leveldb::ReadOptions read_options_;
};
}
