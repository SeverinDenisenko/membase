#pragma once

#include <shared_mutex>

#include <leveldb/db.h>
#include <leveldb/slice.h>

#include "config.hpp"
#include "db.hpp"

namespace mb {

class PersistentDB : public DB {
public:
    PersistentDB(Config& config);
    ~PersistentDB() override;

    ReturnValueResult get(const KeyType&& key) noexcept override;
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
