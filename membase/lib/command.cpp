#include "command.hpp"

#include <fmt/format.h>

std::unordered_map<std::string, mb::Command::Action> mb::Command::actions = {
    { "GET", mb::Command::Action::GET },
    { "PUT", mb::Command::Action::PUT },
    { "REMOVE", mb::Command::Action::REMOVE },
    { "WIPE", mb::Command::Action::WIPE },
    { "FINDKEY", mb::Command::Action::FINDKEY },
    { "FINDVALUE", mb::Command::Action::FINDVALUE }
};

std::unordered_map<mb::Command::Action, mb::Command::Requirement> mb::Command::commands = {
    { mb::Command::Action::GET, { 2 } },
    { mb::Command::Action::PUT, { 3 } },
    { mb::Command::Action::REMOVE, { 2 } },
    { mb::Command::Action::WIPE, { 1 } },
    { mb::Command::Action::FINDKEY, { 2 } },
    { mb::Command::Action::FINDVALUE, { 2 } }
};

mb::Command::Command(Tokens tokens) noexcept
    : tokens(tokens)
{
}

mb::Status mb::Command::Verify() noexcept
{
    if (tokens.size() == 0) {
        return Status::Error();
    }

    if (!actions.contains(*tokens.begin())) {
        return Status::Error();
    }

    action = actions.at(*tokens.begin());

    if (commands[action].tokens == tokens.size()) {
        return Status::Ok();
    } else {
        return Status::Error();
    }
}

mb::Status mb::Command::Run(DB& db) noexcept
{
    auto iter = tokens.begin();
    ++iter;

    switch (action) {
    case Action::GET: {
        std::optional<std::string> ans = db.get(std::move(*iter));
        if (!ans)
            return Status::Error();
        result = fmt::format("VALUE {}\n", *ans);
        return Status::Fine();
    }
    case Action::PUT:
        db.put(std::move(*iter++), std::move(*iter));
        return Status::Ok();
    case Action::REMOVE:
        db.remove(std::move(*iter));
        return Status::Ok();
    case Action::WIPE:
        db.wipe();
        return Status::Ok();
    case Action::FINDKEY: {
        auto data = db.findKey(std::move(*iter));
        for (auto& a : data) {
            result += fmt::format("KEY {}\n", a);
        }
        return Status::Ok();
    }
    case Action::FINDVALUE: {
        auto data = db.findValue(std::move(*iter));
        for (auto& a : data) {
            result += fmt::format("KEY {}\n", a);
        }
        return Status::Ok();
    }
    default:
        return Status::Error();
    }
}

std::string& mb::Command::Result() noexcept {
    return result;
}

