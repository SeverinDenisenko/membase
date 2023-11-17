#pragma once

#include <set>

#include "defines.hpp"
#include "result.hpp"
#include "string.hpp"
#include "tracking_allocator.hpp"

namespace mb {
using Allocator = std::allocator<std::byte>;
using KeyType = StringView;
using ValueType = StringView;

using ReturnValueType = String;
using ReturnValueResult = Result<ReturnValueType>;

using FindResult = std::set<String>;

using InternalAllocator = TrackingAllocator<std::byte>;
using InternalKeyType = BasicString<InternalAllocator>;
using InternalValueType = BasicString<InternalAllocator>;

inline std::size_t hash_value(InternalKeyType const& str)
{
    return str.Hash();
}
}
