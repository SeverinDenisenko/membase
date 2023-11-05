#pragma once

#include "config.hpp"
#include "handler.hpp"
#include "db.hpp"

namespace mb {
class App {
public:
    App(int argc, char* argv[]) noexcept;

    void Run() noexcept;

    ~App();

private:
    Config config;
    std::unique_ptr<DB> db;
    std::unique_ptr<Handler> handler;
};
}
