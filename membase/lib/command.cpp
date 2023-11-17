#include "command.hpp"
#include "string.hpp"
#include "string_helpers.hpp"

#include <fmt/format.h>
#include <string>

const std::unordered_map<std::string, mb::Command::Action> mb::Command::actions = {
    { "GET", mb::Command::Action::GET },
    { "PUT", mb::Command::Action::PUT },
    { "REMOVE", mb::Command::Action::REMOVE },
    { "WIPE", mb::Command::Action::WIPE },
    { "FINDKEY", mb::Command::Action::FINDKEY },
    { "FINDVALUE", mb::Command::Action::FINDVALUE }
};

const std::unordered_map<mb::Command::Action, mb::Command::Requirement> mb::Command::commands = {
    { mb::Command::Action::GET, { 2 } },
    { mb::Command::Action::PUT, { 3 } },
    { mb::Command::Action::REMOVE, { 2 } },
    { mb::Command::Action::WIPE, { 1 } },
    { mb::Command::Action::FINDKEY, { 2 } },
    { mb::Command::Action::FINDVALUE, { 2 } }
};

mb::Command::Command(Tokens& tokens) noexcept
    : tokens(tokens)
{
}

mb::Status mb::Command::Verify() noexcept
{
    if (tokens.Length() == 0) {
        return Status::Error();
    }

    auto cmd = tokens[0].ToStdString();

    if (!actions.contains(cmd)) {
        return Status::Error();
    }
    action = actions.at(cmd);

    if (commands.at(action).tokens == tokens.Length()) {
        return Status::Ok();
    } else {
        return Status::Error();
    }
}

mb::Status mb::Command::Run(DB& db) noexcept
{
    switch (action) {
    case Action::GET: {
        auto ans = db.get(tokens[1]);
        if (!ans)
            return ans;
        result = fmt::format("VALUE {}\n", ans.Value().ToStdString());
        return Status::Fine();
    }
    case Action::PUT:
        return db.put(tokens[1], tokens[2]);
    case Action::REMOVE:
        return db.remove(tokens[1]);
    case Action::WIPE:
        return db.wipe();
    case Action::FINDKEY: {
        auto data = db.findKey(tokens[1]);
        for (auto& a : data) {
            result += fmt::format("KEY {}\n", a.ToStdString());
        }
        return Status::Ok();
    }
    case Action::FINDVALUE: {
        auto data = db.findValue(tokens[1]);
        for (auto& a : data) {
            result += fmt::format("KEY {}\n", a.ToStdString());
        }
        return Status::Ok();
    }
    default:
        return Status::Error();
    }
}

std::string& mb::Command::Result() noexcept
{
    return result;
}
