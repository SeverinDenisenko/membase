#pragma once

#include "db.hpp"

#include <boost/unordered/unordered_flat_map.hpp>

namespace mb {

class MemoryDB : public DB {
public:
    MemoryDB();

    std::optional<ValueType> get(const KeyType&& key) noexcept override;
    void put(const KeyType&& key, const ValueType&& value) noexcept override;
    void remove(const KeyType&& key) noexcept override;

private:
    boost::unordered_flat_map<KeyType, ValueType> map;
};
}
