#pragma once

#include <boost/asio.hpp>

#include "Settings.h"

class BaseClient {
public:
    BaseClient(boost::asio::io_service &service,
               std::function<void(std::string command, BaseClient *this_client)> command_handler,
               std::function<void(BaseClient *client)> on_delete);

    boost::asio::ip::tcp::socket &getSock();

    virtual void afterConnect();

    virtual void writeCommand(std::string &command);

protected:
    virtual void readCommandHandler(const boost::system::error_code &err, std::size_t bytes_transferred);

    virtual std::size_t checkEndOfRead(const boost::system::error_code &err, std::size_t bytes_transferred);

    virtual void writeCommandHandler(const boost::system::error_code &err, std::size_t bytes_transferred);

    virtual void readCommand();

    boost::asio::ip::tcp::socket sock_;
    std::function<void(std::string command, BaseClient *this_client)> commandHandler_;
    std::function<void(BaseClient *client)> onDelete_;
    char readBuf_[BUFFER_LENGTH];
    char sendBuf_[BUFFER_LENGTH];
};