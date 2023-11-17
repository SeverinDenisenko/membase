#include "memory_db.hpp"
#include "comparator.hpp"
#include "db.hpp"
#include "string_helpers.hpp"

mb::MemoryDB::MemoryDB(Config& config)
    : config_(config)
    , map_(config_.buckets)
{
    allocatorsMemory = AllocatorsMemory(config_.memory);
}

mb::ReturnValueResult mb::MemoryDB::get(const mb::KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);

    if (!allocatorsMemory.IsEnoughFor(key.Length())) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return ReturnValueResult::Error();
    }

    InternalKeyType key_internal { key };

    auto find = map_.find(key_internal);

    if (find == map_.end()) {
        return ReturnValueResult::Error();
    } else {
        return ReturnValueResult::Ok(find->second.Copy<Allocator>());
    }
}

mb::Status mb::MemoryDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    if (!allocatorsMemory.IsEnoughFor(key.Length() + value.Length())) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Status::Error();
    }

    InternalKeyType key_internal { key };
    InternalValueType value_internal { value };

    map_.insert(std::pair<InternalKeyType, InternalValueType> { std::move(key_internal), std::move(value_internal) });

    return Status::Ok();
}

mb::Status mb::MemoryDB::remove(const KeyType&& key) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex_);

    if (!allocatorsMemory.IsEnoughFor(key.Length() + 1)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Status::Error();
    }

    InternalKeyType key_internal { key };

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
    InternalKeyType key_internal { key };

    for (auto& pair : map_) {
        if (comparePrefix(key_internal, pair.first)) {
            result.insert(pair.first.Copy<Allocator>());
        }
    }

    return result;
}

mb::FindResult mb::MemoryDB::findValue(const ValueType&& value) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex_);
    FindResult result;

    InternalValueType value_internal { value };

    for (auto& pair : map_) {
        if (comparePrefix(value_internal, pair.second)) {
            result.insert(pair.first.Copy<Allocator>());
        }
    }

    return result;
}
