#pragma once

#include <string>

namespace mb {
class Handler {
public:
    std::string operator()(const std::string& requst) noexcept;
};
}
