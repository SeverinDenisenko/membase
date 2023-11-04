#pragma once

#include "config.hpp"
#include "memory_db.hpp"
#include "handler.hpp"

namespace mb {
class App {
public:
    App(int argc, char* argv[]) noexcept;

    void Run() noexcept;

    ~App();

private:
    Config config;
    std::unique_ptr<DB> db;
    Handler handler;
};
}
