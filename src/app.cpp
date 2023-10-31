#include "app.hpp"

#include <fstream>

#include <boost/chrono/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <fmt/format.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "config.hpp"
#include "server.hpp"

DEFINE_string(config, "config.json", "Main config");

mb::App::App(int argc, char* argv[]) noexcept
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InstallFailureSignalHandler();
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    try {
        nlohmann::json json;
        std::fstream file(FLAGS_config);
        file >> json;

        config.host = json["host"];
        config.port = json["port"];
        config.max_request_length = json["max_request_length"];
        config.memory = json["memory"];
    } catch (const std::exception& e) {
        LOG(FATAL) << fmt::format("Can't load config! Reason: {}", e.what());
    }

    LOG(INFO) << "Starting Membase...";
}

mb::App::~App()
{
    LOG(INFO) << "Exiting Membase...";
}

void mb::App::Run() noexcept
try {
    boost::asio::io_context io_context;
    Server server(io_context, config, handler);
    io_context.run();

    boost::thread(boost::bind(&boost::asio::io_service::run, &io_context)).detach();

    boost::this_thread::sleep_for(boost::chrono::seconds(60));
    io_context.stop();
} catch (const std::exception& e) {
    LOG(FATAL) << fmt::format("Excepton: {}.", e.what());
}
