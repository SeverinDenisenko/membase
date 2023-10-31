#include <fmt/format.h>
#include <glog/logging.h>

#include "server.hpp"

mb::Server::Server(boost::asio::io_context& io_context, const Config& config)
    : config(config)
    , io_context_(io_context)
    , acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(config.host), config.port))
{
    LOG(INFO) << fmt::format("Server created.");
    start_accept();
}

mb::Server::~Server()
{
    LOG(INFO) << fmt::format("Server destroyed.");
}

void mb::Server::start_accept()
{
    boost::shared_ptr<Connection> new_connection = Connection::create(io_context_, config);

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&Server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));
}

void mb::Server::handle_accept(boost::shared_ptr<Connection> new_connection,
    const boost::system::error_code& error)
{
    if (!error) {
        new_connection->start();
    } else {
        LOG(ERROR) << fmt::format("Error handling accept: {}", error.message());
    }

    start_accept();
}

