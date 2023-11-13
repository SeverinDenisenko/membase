#pragma once

#include <optional>
#include <set>
#include <string>

#include "result.hpp"
#include "status.hpp"

namespace mb {
using KeyType = std::string;
using ValueType = std::string;
using FindResult = std::set<mb::KeyType>;

class DB {
public:
    virtual Result<ValueType> get(const KeyType&& key) noexcept = 0;
    virtual Status put(const KeyType&& key, const ValueType&& value) noexcept = 0;
    virtual Status remove(const KeyType&& key) noexcept = 0;
    virtual Status wipe() noexcept = 0;
    virtual FindResult findKey(const KeyType&& key) noexcept = 0;
    virtual FindResult findValue(const ValueType&& value) noexcept = 0;

    virtual ~DB() = default;
};
}
