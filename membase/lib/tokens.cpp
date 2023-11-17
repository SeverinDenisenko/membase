#include "tokens.hpp"
#include "defines.hpp"
#include "string.hpp"

mb::Tokens::Tokens(StringView str) noexcept
    : str_(str)
{
    String::Iterator last = str_.Begin();

    for (String::Iterator current = str_.Begin(); current < str_.End(); ++current) {
        if (current == str_.End() - 1) {
            tokens_.Emplace(last, str_.End());
            break;
        }

        if (*current == sep_) {
            tokens_.Emplace(last, current);
            last = current;
            ++last;
        }
    }
}

size_t mb::Tokens::Length() const noexcept
{
    return tokens_.Length();
}

mb::StringView mb::Tokens::operator[](size_t index) noexcept
{
    return tokens_[index];
}
