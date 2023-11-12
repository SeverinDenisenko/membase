#pragma once

#include <optional>
#include <string>
#include <unordered_set>

#include "result.hpp"
#include "status.hpp"

namespace mb {
using KeyType = std::string;
using ValueType = std::string;

class DB {
public:
    virtual Result<ValueType> get(const KeyType&& key) noexcept = 0;
    virtual Status put(const KeyType&& key, const ValueType&& value) noexcept = 0;
    virtual Status remove(const KeyType&& key) noexcept = 0;
    virtual Status wipe() noexcept = 0;
    virtual std::unordered_set<KeyType> findKey(const KeyType&& key) noexcept = 0;
    virtual std::unordered_set<KeyType> findValue(const ValueType&& value) noexcept = 0;

    virtual ~DB() = default;
};
}
