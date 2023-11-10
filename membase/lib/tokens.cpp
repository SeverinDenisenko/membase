#include "tokens.hpp"

mb::Tokens::Tokens(const std::string& str) noexcept
    : _sep(" ")
    , _tokenizer(str, _sep)
{
    _begin = _tokenizer.begin();
    _end = _tokenizer.end();
}

mb::Tokens::iterator_t mb::Tokens::begin() const noexcept
{
    return _begin;
}

mb::Tokens::iterator_t mb::Tokens::end() const noexcept
{
    return _end;
}

size_t mb::Tokens::size() const noexcept
{
    return std::distance(_begin, _end);
}
