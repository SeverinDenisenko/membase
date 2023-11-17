#pragma once

#include <string>

#include "string.hpp"

namespace mb {

inline bool comparePrefix(const std::string& search, const std::string& value) noexcept
{
    if (search.size() > value.size())
        return false;

    size_t size = search.size();

    for (size_t i = 0; i < size; ++i) {
        if (search[i] != value[i]) {
            return false;
        }
    }

    return true;
}

template <typename StringType>
inline bool comparePrefix(const StringType& search, const StringType& value) noexcept
{
    if (search.Length() > value.Length())
        return false;

    size_t size = search.Length();

    for (size_t i = 0; i < size; ++i) {
        if (search[i] != value[i]) {
            return false;
        }
    }

    return true;
}
} // namespace mb
