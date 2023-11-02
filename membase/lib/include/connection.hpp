#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "config.hpp"
#include "handler.hpp"

namespace mb {
class Connection : public boost::enable_shared_from_this<Connection> {
public:
    static boost::shared_ptr<Connection>
    create(boost::asio::io_context& io_context, const Config& config, Handler& handler) noexcept;

    void start() noexcept;
    boost::asio::ip::tcp::socket& socket() noexcept;

    ~Connection();

private:
    Connection(boost::asio::io_context& io_context, const Config& config, Handler& handler) noexcept;

    void request() noexcept;
    void response() noexcept;

    void handle_request(const boost::system::error_code& error, size_t size) noexcept;
    void handle_response(const boost::system::error_code& error, size_t size) noexcept;

    const Config& config;
    Handler& handler;
    std::string request_ = std::string(1024, '\0');
    std::string response_;
    boost::asio::ip::tcp::socket socket_;
};
} // namespace mb
