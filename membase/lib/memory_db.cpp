#include "memory_db.hpp"

mb::MemoryDB::MemoryDB()
{
}

std::optional<mb::ValueType> mb::MemoryDB::get(const KeyType&& key) noexcept
{
    auto find = map.find(key);

    if (find == map.end()) {
        return std::nullopt;
    } else {
        return find->second;
    }
}

void mb::MemoryDB::put(const KeyType&& key, const ValueType&& value) noexcept
{
    map[key] = value;
}

void mb::MemoryDB::remove(const KeyType&& key) noexcept
{
    auto find = map.find(key);

    if (find != map.end()) {
        map.erase(find);
    }
}
