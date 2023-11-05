#include "memory_db.hpp"
#include "db.hpp"

mb::MemoryDB::MemoryDB(Config& config)
    : config(config)
{
}

std::optional<mb::ValueType> mb::MemoryDB::get(const mb::KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);

    auto find = map.find(key);

    if (find == map.end()) {
        return std::nullopt;
    } else {
        return find->second;
    }
}

void mb::MemoryDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    map[key] = value;
}

void mb::MemoryDB::remove(const KeyType&& key) noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    auto find = map.find(key);

    if (find != map.end()) {
        map.erase(find);
    }
}

void mb::MemoryDB::wipe() noexcept
{
    std::lock_guard<std::shared_mutex> lock(mutex);

    map.clear();
}

std::unordered_set<mb::KeyType> mb::MemoryDB::findKey(const KeyType&& key) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);

    std::unordered_set<mb::KeyType> result;

    for (auto pair : map) {
        size_t len = std::min(key.length(), pair.first.length());
        if (pair.first.substr(0, len) == key.substr(0, len)) {
            result.emplace(pair.first);
        }
    }

    return result;
}

std::unordered_set<mb::KeyType> mb::MemoryDB::findValue(const ValueType&& value) noexcept
{
    std::shared_lock<std::shared_mutex> lock(mutex);

    std::unordered_set<mb::ValueType> result;

    for (auto pair : map) {
        size_t len = std::min(value.length(), pair.second.length());
        if (pair.second.substr(0, len) == value.substr(0, len)) {
            result.emplace(pair.first);
        }
    }

    return result;
}
