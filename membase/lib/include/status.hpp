#pragma once

namespace mb {
class Status {
public:
    static Status Ok() noexcept;
    static Status Fine() noexcept;
    static Status Error() noexcept;

    operator bool() const;
    const char* Message() const;

private:
    explicit Status(const char* msg, bool ok) noexcept;

    const char* msg;
    bool ok;
};
}
