#pragma once

#include "config.hpp"
#include "handler.hpp"

namespace mb {
class App {
public:
    App(int argc, char* argv[]) noexcept;

    void Run() noexcept;

    ~App();

private:
    Config config;
    Handler handler;
};
}
