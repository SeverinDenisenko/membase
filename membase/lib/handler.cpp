#include "handler.hpp"

#include <boost/tokenizer.hpp>
#include <fmt/format.h>
#include <glog/logging.h>

#include "command.hpp"
#include "string_helpers.hpp"
#include "tokens.hpp"

mb::Handler::Handler(DB& db) noexcept
    : db(db)
{
}

std::string mb::Handler::operator()(const String& request) noexcept
{
    Tokens tokens(Truncate(request));
    Command command(tokens);

    Status status = command.Verify();
    if (!status) {
        LOG(WARNING) << "Invalid command.";
        return status.Message();
    }

    status = command.Run(db);
    if (!status) {
        LOG(WARNING) << "Command exited with error.";
        return status.Message();
    }

    return fmt::format("{}{}", command.Result(), status.Message());
}
