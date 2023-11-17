#include <fmt/format.h>
#include <glog/logging.h>

#include "connection.hpp"

boost::shared_ptr<mb::Connection>
mb::Connection::create(boost::asio::io_context& io_context, const Config& config, Handler& handler) noexcept
try {
    return boost::shared_ptr<Connection>(new Connection(io_context, config, handler));
} catch (const std::exception& e) {
    LOG(FATAL) << "Can't create connection: " << e.what();
}

void mb::Connection::start() noexcept
{
    LOG(INFO) << "Connection started.";
    request();
}

boost::asio::ip::tcp::socket& mb::Connection::socket() noexcept
{
    return socket_;
}

mb::Connection::Connection(boost::asio::io_context& io_context, const Config& config, Handler& handler) noexcept
    : config_(config)
    , handler(handler)
    , request_(config_.max_request_length)
    , response_()
    , socket_(io_context)
{
    LOG(INFO) << "Connection created.";
}

mb::Connection::~Connection()
{
    LOG(INFO) << "Connection destroyed.";
}

void mb::Connection::request() noexcept
try {
    request_.Reset();

    socket_.async_read_some(
        boost::asio::buffer(request_.Data(), request_.Length()),
        boost::bind(&Connection::handle_request, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
} catch (const std::exception& e) {
    LOG(ERROR) << "Exception in handling request: " << e.what();
}

void mb::Connection::response() noexcept
try {
    response_ = handler(request_);

    boost::asio::async_write(
        socket_, boost::asio::buffer(response_),
        boost::bind(&Connection::handle_response, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
} catch (const std::exception& e) {
    LOG(ERROR) << "Exception in handling request: " << e.what();
}

void mb::Connection::handle_request(const boost::system::error_code& error, size_t /* size */) noexcept
{
    if (error == boost::asio::error::eof) {
        return;
    }

    if (error) {
        LOG(ERROR) << fmt::format("Error handling request: {}", error.message());
    } else {
        response();
    }
}

void mb::Connection::handle_response(const boost::system::error_code& error, size_t /* size */) noexcept
{
    if (error) {
        LOG(ERROR) << fmt::format("Error handling response: {}", error.message());
    } else {
        request();
    }
}
