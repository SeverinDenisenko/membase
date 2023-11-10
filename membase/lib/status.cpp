#include "status.hpp"

mb::Status::Status(std::string msg, bool ok) noexcept
    : msg(msg)
    , ok(ok)
{
}

mb::Status mb::Status::Ok() noexcept
{
    return Status("OK\n", true);
}

mb::Status mb::Status::Fine() noexcept
{
    return Status("", true);
}

mb::Status mb::Status::Error() noexcept
{
    return Status("ERROR\n", false);
}

mb::Status::operator bool() const
{
    return ok;
}

std::string mb::Status::Message() const
{
    return msg;
}
