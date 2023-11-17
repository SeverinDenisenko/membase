#pragma once

#include <string>
#include <unordered_map>

#include "db.hpp"
#include "status.hpp"
#include "tokens.hpp"

namespace mb {

class Command {
public:
    explicit Command(Tokens& tokens) noexcept;
    Status Verify() noexcept;
    Status Run(DB& db) noexcept;
    std::string& Result() noexcept;

private:
    enum class Action : size_t {
        GET,
        PUT,
        REMOVE,
        WIPE,
        FINDKEY,
        FINDVALUE
    };

    struct Requirement {
        size_t tokens;
    };

    Action action;
    Tokens& tokens;
    std::string result;

    static const std::unordered_map<Action, Requirement> commands;
    static const std::unordered_map<std::string, Action> actions;
};
}
