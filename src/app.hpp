#pragma once

#include "config.hpp"

namespace mb {
class App {
public:
    App(int argc, char* argv[]) noexcept;

    void Run() noexcept;

    ~App();

private:
    Config config;
};
}
