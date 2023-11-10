#include "handler.hpp"

#include <boost/tokenizer.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include "command.hpp"
#include "tokens.hpp"

mb::Handler::Handler(DB& db) noexcept
    : db(db)
{
}

std::string mb::Handler::operator()(const std::string& request) noexcept
{
    std::string truncated = truncate(request);
    Tokens tokens(truncated);
    Command command(tokens);

    Status status = command.Verify();
    if (!status) {
        return status.Message();
    }

    status = command.Run(db);
    if (!status) {
        return status.Message();
    }

    return fmt::format("{}{}", command.Result(), status.Message());
}

std::string mb::Handler::truncate(const std::string& request) noexcept
{
    std::string res;

    auto it = std::find_if(request.begin(), request.end(), [](const char c) {
        return std::iscntrl(c);
    });

    if (it == request.end()) {
        return res;
    }

    size_t len = std::distance(request.begin(), it);
    res = request.substr(0, len);

    return res;
}
