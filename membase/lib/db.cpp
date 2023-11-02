#include "db.hpp"

mb::DB::DB()
{
}

std::optional<std::string> mb::DB::get(const std::string& key) noexcept
{
    auto find = map.find(key);

    if (find == map.end()) {
        return std::nullopt;
    } else {
        return find->second;
    }
}

void mb::DB::put(const std::string& key, const std::string& value) noexcept
{
    map[key] = value;
}

void mb::DB::remove(const std::string& key) noexcept
{
    auto find = map.find(key);

    if (find != map.end()) {
        map.erase(find);
    }
}
