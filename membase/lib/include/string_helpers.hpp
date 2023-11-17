#pragma once

#include "string.hpp"
#include <cctype>
#include <cstddef>

namespace mb {

inline StringView Truncate(const String& str) noexcept
{
    String::Iterator current = str.Begin();
    String::Iterator end = str.End();

    while (current < end) {
        if (std::iscntrl(char(*current))) {
            return StringView { str.Begin(), current };
        }
        ++current;
    }

    return StringView { str.Begin(), str.End() };
}

}
