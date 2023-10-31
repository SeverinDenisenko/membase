#pragma once

#include "config.hpp"
#include "db.hpp"
#include "handler.hpp"

namespace mb {
class App {
public:
    App(int argc, char* argv[]) noexcept;

    void Run() noexcept;

    ~App();

private:
    Config config;
    DB db;
    Handler handler;
};
}
