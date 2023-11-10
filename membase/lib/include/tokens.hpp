#pragma once

#include <boost/tokenizer.hpp>

namespace mb {
class Tokens {
public:
    using tokenizer_t = boost::tokenizer<boost::char_separator<char>>;
    using iterator_t = tokenizer_t::iterator;

    explicit Tokens(const std::string& str) noexcept;

    iterator_t begin() const noexcept;
    iterator_t end() const noexcept;
    size_t size() const noexcept;

public:
    boost::char_separator<char> _sep;
    tokenizer_t _tokenizer;
    iterator_t _begin;
    iterator_t _end;
};
}
