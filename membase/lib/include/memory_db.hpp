#pragma once

#include "allocator.hpp"
#include "config.hpp"
#include "db.hpp"

#include <boost/unordered/unordered_flat_map.hpp>
#include <shared_mutex>

namespace mb {
using InternalKeyType = std::basic_string<char, std::char_traits<char>, Allocator<char>>;
using InternalValueType = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

class MemoryDB : public DB {
public:
    MemoryDB(Config& config);

    Result<ValueType> get(const KeyType&& key) noexcept override;
    Status put(const KeyType&& key, const ValueType&& value) noexcept override;
    Status remove(const KeyType&& key) noexcept override;
    Status wipe() noexcept override;
    FindResult findKey(const KeyType&& key) noexcept override;
    FindResult findValue(const ValueType&& value) noexcept override;

private:
    Config& config_;
    std::shared_mutex mutex_;
    boost::unordered_flat_map<InternalKeyType, InternalValueType> map_;
};
}
