#include "memory_db.hpp"
#include "comparator.hpp"
#include "db.hpp"

mb::MemoryDB::MemoryDB(Config& config)
    : config(config)
    , map(config.buckets)
{
    allocatorsMemory = AllocatorsMemory(config.memory);
}

mb::Result<mb::ValueType> mb::MemoryDB::get(const mb::KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);

    if (!allocatorsMemory.IsEnoughFor(key.size() + 1)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Result<ValueType>::Error();
    }

    auto key_internal = InternalKeyType(key);

    auto find = map.find(key_internal);

    if (find == map.end()) {
        return Result<ValueType>::Error();
    } else {
        return Result<ValueType>::Ok(ValueType(find->second));
    }
}

mb::Status mb::MemoryDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    if (!allocatorsMemory.IsEnoughFor(key.size() + value.size() + 2)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Status::Error();
    }

    auto key_internal = InternalKeyType(key);
    auto value_internal = InternalValueType(value);

    map.emplace(key_internal, value_internal);

    return Status::Ok();
}

mb::Status mb::MemoryDB::remove(const KeyType&& key) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    if (!allocatorsMemory.IsEnoughFor(key.size() + 1)) {
        LOG(WARNING) << fmt::format("MemoryDB is full!");
        return Status::Error();
    }

    auto key_internal = InternalKeyType(key);

    auto find = map.find(key_internal);

    if (find != map.end()) {
        map.erase(find);
    }

    return Status::Ok();
}

mb::Status mb::MemoryDB::wipe() noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    map.clear();

    return Status::Ok();
}

mb::FindResult mb::MemoryDB::findKey(const KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    FindResult result;
    Comparator<InternalValueType> comparator;

    auto key_internal = InternalKeyType(key);

    for (auto pair : map) {
        if (comparator.comparePrefix(key_internal, pair.first)) {
            result.emplace(pair.first);
        }
    }

    return result;
}

mb::FindResult mb::MemoryDB::findValue(const ValueType&& value) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    FindResult result;
    Comparator<InternalValueType> comparator;

    auto value_internal = InternalValueType(value);

    for (auto pair : map) {
        if (comparator.comparePrefix(value_internal, pair.second)) {
            result.emplace(pair.first);
        }
    }

    return result;
}
