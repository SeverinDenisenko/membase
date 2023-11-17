#pragma once

#include <utility>

#include "status.hpp"

namespace mb {
template <typename T>
class Result {
public:
    Result(Result&& other) noexcept
    {
        value = std::move(other.value);
        status = std::move(other.status);
    }

    static Result Ok(T&& value) noexcept
    {
        return Result(std::move(value), Status::Ok());
    }

    static Result Error() noexcept
    {
        return Result(Status::Error());
    }

    operator Status() const noexcept
    {
        return status;
    }

    operator bool() const noexcept
    {
        return status;
    }

    T& Value() noexcept
    {
        return value;
    }

private:
    Result(T&& value, Status status) noexcept
        : status(std::move(status))
        , value(std::move(value))
    {
    }

    Result(Status&& status) noexcept
        : status(std::move(status))
    {
    }

    Status status;
    T value;
};
} // namespace mb
