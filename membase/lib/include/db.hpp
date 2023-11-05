#pragma once

#include <optional>
#include <string>
#include <unordered_set>

namespace mb {
using KeyType = std::string;
using ValueType = std::string;

class DB {
public:
    virtual std::optional<ValueType> get(const KeyType&& key) noexcept = 0;
    virtual void put(const KeyType&& key, const ValueType&& value) noexcept = 0;
    virtual void remove(const KeyType&& key) noexcept = 0;
    virtual void wipe() noexcept = 0;
    virtual std::unordered_set<KeyType> findKey(const KeyType&& key) noexcept = 0;
    virtual std::unordered_set<KeyType> findValue(const ValueType&& value) noexcept = 0;

    virtual ~DB() = default;
};
}

