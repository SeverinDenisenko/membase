#pragma once

#include "defines.hpp"
#include "string.hpp"
#include "vector.hpp"

namespace mb {
class Tokens {
public:
    explicit Tokens(StringView str) noexcept;

    size_t Length() const noexcept;
    StringView operator[](size_t index) noexcept;

private:
    std::byte sep_ { ' ' };
    StringView str_ {};
    Vector<StringView> tokens_ { 0, 3 };
};
}
