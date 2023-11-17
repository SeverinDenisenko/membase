#include "persistent_db.hpp"
#include "comparator.hpp"
#include "db.hpp"

#include <fmt/format.h>
#include <glog/logging.h>
#include <leveldb/write_batch.h>

mb::PersistentDB::PersistentDB(Config& config)
    : config_(config)
{
    options_.create_if_missing = config_.create_if_missing;
    leveldb::Status s = leveldb::DB::Open(options_, "/tmp/testdb", &db_);
    write_options_.sync = true;

    if (!s.ok()) {
        LOG(FATAL) << fmt::format("Can't open Persistent DB. Reason: {}", s.ToString());
    }
}

mb::PersistentDB::~PersistentDB()
{
    delete db_;
}

mb::ReturnValueResult mb::PersistentDB::get(const KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::string result;

    leveldb::Status s = db_->Get(read_options_, key.ToSlice(), &result);

    if (!s.ok()) {
        return ReturnValueResult::Error();
    } else {
        return ReturnValueResult::Ok(result);
    }
}

mb::Status mb::PersistentDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    leveldb::Status s = db_->Put(write_options_, key.ToSlice(), value.ToSlice());

    if (s.ok()) {
        return Status::Ok();
    } else {
        return Status::Error();
    }
}

mb::Status mb::PersistentDB::remove(const KeyType&& key) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    leveldb::Status s = db_->Delete(write_options_, key.ToSlice());

    if (s.ok()) {
        return Status::Ok();
    } else {
        return Status::Error();
    }
}

mb::Status mb::PersistentDB::wipe() noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    leveldb::WriteBatch batch;

    leveldb::Iterator* it = db_->NewIterator(read_options_);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        batch.Delete(it->key());
    }
    delete it;

    leveldb::Status s = db_->Write(write_options_, &batch);

    if (s.ok()) {
        return Status::Ok();
    } else {
        return Status::Error();
    }
}

mb::FindResult mb::PersistentDB::findKey(const KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    FindResult result;

    leveldb::Iterator* it = db_->NewIterator(read_options_);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (comparePrefix(key.ToSlice().ToString(), it->key().ToString())) {
            result.emplace(it->key().ToString());
        }
    }
    delete it;

    return result;
}

mb::FindResult mb::PersistentDB::findValue(const ValueType&& value) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    FindResult result;

    leveldb::Iterator* it
        = db_->NewIterator(read_options_);
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (comparePrefix(value.ToSlice().ToString(), it->value().ToString())) {
            result.emplace(it->key().ToString());
        }
    }
    delete it;

    return result;
}
