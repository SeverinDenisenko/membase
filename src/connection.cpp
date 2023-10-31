#include <fmt/format.h>
#include <glog/logging.h>

#include "connection.hpp"

boost::shared_ptr<mb::Connection>
mb::Connection::create(boost::asio::io_context& io_context, const Config& config) noexcept
try {
    return boost::shared_ptr<Connection>(new Connection(io_context, config));
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

mb::Connection::Connection(boost::asio::io_context& io_context, const Config& config) noexcept
    : config(config)
    , request_(config.max_request_length, '\0')
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
    socket_.async_read_some(
        boost::asio::buffer(request_, config.max_request_length),
        boost::bind(&Connection::handle_request, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
} catch (const std::exception& e){
    LOG(ERROR) << "Exception in handling request: " << e.what();
}

void mb::Connection::response() noexcept
try {
    response_ = "Hello kitty! ";
    for(char c: request_)
        if(c != '\0' || c != '\n')
            response_ += c;
        else
            break;
    response_ += "!\n";

    boost::asio::async_write(
        socket_, boost::asio::buffer(response_),
        boost::bind(&Connection::handle_response, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
} catch (const std::exception& e){
    LOG(ERROR) << "Exception in handling request: " << e.what();
}

void mb::Connection::handle_request(const boost::system::error_code& error, size_t size) noexcept
{
    if (error) {
        LOG(ERROR) << fmt::format("Error handling request: {}", error.message());
    } else {
        LOG(INFO) << fmt::format("Request handled. {} bytes transfered.", size);
        response();
    }
}

void mb::Connection::handle_response(const boost::system::error_code& error, size_t size) noexcept
{
    if (error) {
        LOG(ERROR) << fmt::format("Error handling response: {}", error.message());
    } else {
        LOG(INFO) << fmt::format("Response handled. {} bytes transfered.", size);
        request();
    }
}

