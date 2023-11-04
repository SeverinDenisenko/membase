#pragma once

#include <optional>
#include <string>

namespace mb {
using KeyType = std::string;
using ValueType = std::string;

class DB {
public:
    virtual std::optional<ValueType> get(const KeyType&& key) noexcept = 0;
    virtual void put(const KeyType&& key, const ValueType&& value) noexcept = 0;
    virtual void remove(const KeyType&& key) noexcept = 0;

    virtual ~DB() = default;
};
}

