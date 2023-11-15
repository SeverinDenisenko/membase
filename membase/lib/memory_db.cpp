#include "memory_db.hpp"
#include "comparator.hpp"
#include "db.hpp"

mb::MemoryDB::MemoryDB(Config& config)
    : config_(config)
    , map_(config_.buckets)
{
    allocatorsMemory = AllocatorsMemory(config_.memory);
}

mb::Result<mb::ValueType> mb::MemoryDB::get(const mb::KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);

    if (!allocatorsMemory.IsEnoughFor(key.size() + 1)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Result<ValueType>::Error();
    }

    auto key_internal = InternalKeyType(key);

    auto find = map_.find(key_internal);

    if (find == map_.end()) {
        return Result<ValueType>::Error();
    } else {
        return Result<ValueType>::Ok(ValueType(find->second));
    }
}

mb::Status mb::MemoryDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    if (!allocatorsMemory.IsEnoughFor(key.size() + value.size() + 2)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Status::Error();
    }

    auto key_internal = InternalKeyType(key);
    auto value_internal = InternalValueType(value);

    map_.emplace(key_internal, value_internal);

    return Status::Ok();
}

mb::Status mb::MemoryDB::remove(const KeyType&& key) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    if (!allocatorsMemory.IsEnoughFor(key.size() + 1)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Status::Error();
    }

    auto key_internal = InternalKeyType(key);

    auto find = map_.find(key_internal);

    if (find != map_.end()) {
        map_.erase(find);
    }

    return Status::Ok();
}

mb::Status mb::MemoryDB::wipe() noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    map_.clear();

    return Status::Ok();
}

mb::FindResult mb::MemoryDB::findKey(const KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    FindResult result;
    Comparator<InternalValueType> comparator;

    auto key_internal = InternalKeyType(key);

    for (auto pair : map_) {
        if (comparator.comparePrefix(key_internal, pair.first)) {
            result.emplace(pair.first);
        }
    }

    return result;
}

mb::FindResult mb::MemoryDB::findValue(const ValueType&& value) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    FindResult result;
    Comparator<InternalValueType> comparator;

    auto value_internal = InternalValueType(value);

    for (auto pair : map_) {
        if (comparator.comparePrefix(value_internal, pair.second)) {
            result.emplace(pair.first);
        }
    }

    return result;
}
