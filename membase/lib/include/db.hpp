#pragma once

#include <set>

#include "result.hpp"
#include "status.hpp"
#include "string.hpp"
#include "string_helpers.hpp"
#include "types.hpp"

namespace mb {
class DB {
public:
    virtual ReturnValueResult get(const KeyType&& key) noexcept = 0;
    virtual Status put(const KeyType&& key, const ValueType&& value) noexcept = 0;
    virtual Status remove(const KeyType&& key) noexcept = 0;
    virtual Status wipe() noexcept = 0;
    virtual FindResult findKey(const KeyType&& key) noexcept = 0;
    virtual FindResult findValue(const ValueType&& value) noexcept = 0;

    virtual ~DB() = default;
};
}
