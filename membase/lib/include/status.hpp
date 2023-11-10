#pragma once

#include <string>

namespace mb {
class Status {
public:
    static Status Ok() noexcept;
    static Status Fine() noexcept;
    static Status Error() noexcept;

    operator bool() const;
    std::string Message() const;

private:
    explicit Status(std::string msg, bool ok) noexcept;

    std::string msg;
    bool ok;
};
}
