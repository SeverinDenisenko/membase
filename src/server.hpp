#pragma once

#include "connection.hpp"
#include "config.hpp"

namespace mb {
class Server {
public:
    Server(boost::asio::io_context& io_context, const Config& config);
    ~Server();

private:
    void start_accept();
    void handle_accept(boost::shared_ptr<Connection> new_connection,
        const boost::system::error_code& error);

    const Config& config;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
}
