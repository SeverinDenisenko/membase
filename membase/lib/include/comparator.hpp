#pragma once

namespace mb {
template <typename String>
class Comparator {
public:
    bool comparePrefix(const String& search, const String& value) const noexcept
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

    bool compareMinimum(const String& search, const String& value) const noexcept
    {
        size_t size = std::min(search.size(), value.size());

        for (size_t i = 0; i < size; ++i) {
            if (search[i] != value[i]) {
                return false;
            }
        }

        return true;
    }
};
} // namespace mb
