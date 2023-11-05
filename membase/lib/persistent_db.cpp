#include "persistent_db.hpp"
#include "db.hpp"

#include <fmt/format.h>
#include <glog/logging.h>
#include <leveldb/write_batch.h>

mb::PersistentDB::PersistentDB(Config& config)
    : config(config)
{
    options.create_if_missing = config.create_if_missing;
    leveldb::Status s = leveldb::DB::Open(options, "/tmp/testdb", &db);
    write_options.sync = true;

    if (!s.ok()) {
        LOG(FATAL) << fmt::format("Can't open Persistent DB. Reason: {}", s.ToString());
    }
}

mb::PersistentDB::~PersistentDB()
{
    delete db;
}

std::optional<mb::ValueType> mb::PersistentDB::get(const mb::KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    std::string result;

    leveldb::Status s = db->Get(read_options, key, &result);

    if (!s.ok()) {
        return std::nullopt;
    } else {
        return result;
    }
}

void mb::PersistentDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    leveldb::Status s = db->Put(write_options, key, value);

    // TODO error handling
}

void mb::PersistentDB::remove(const KeyType&& key) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    leveldb::Status s = db->Delete(write_options, key);
}

void mb::PersistentDB::wipe() noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    leveldb::WriteBatch batch;

    leveldb::Iterator* it = db->NewIterator(read_options);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        batch.Delete(it->key());
    }
    delete it;

    leveldb::Status s = db->Write(write_options, &batch);
}

std::unordered_set<mb::KeyType> mb::PersistentDB::findKey(const KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    std::unordered_set<mb::KeyType> result;

    leveldb::Iterator* it = db->NewIterator(read_options);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        size_t len = std::min(key.length(), it->key().ToString().length());
        if (it->key().ToString().substr(0, len) == key.substr(0, len)) {
            result.emplace(it->key().ToString());
        }
    }
    delete it;

    return result;
}

std::unordered_set<mb::KeyType> mb::PersistentDB::findValue(const ValueType&& value) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    std::unordered_set<mb::ValueType> result;

    leveldb::Iterator* it = db->NewIterator(read_options);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        size_t len = std::min(value.length(), it->value().ToString().length());
        if (it->value().ToString().substr(0, len) == value.substr(0, len)) {
            result.emplace(it->key().ToString());
        }
    }
    delete it;

    return result;
}
