#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "BaseClient.h"


BaseClient::BaseClient(boost::asio::io_service &service, std::function<void(std::string, BaseClient *)> command_handler,
                       std::function<void(BaseClient *)> on_delete) : sock_(service),
                                                                      commandHandler_(std::move(command_handler)),
                                                                      onDelete_(std::move(on_delete)) {
}


void BaseClient::afterConnect() {
    readCommand();
}

void BaseClient::readCommand() {
    boost::asio::async_read(sock_, boost::asio::buffer(readBuf_),
                            boost::bind(&BaseClient::checkEndOfRead, this, boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred),
                            boost::bind(&BaseClient::readCommandHandler, this, boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void BaseClient::writeCommand(std::string &command) {
    for (int i = 0; i < command.length(); ++i) {
        sendBuf_[i] = command[i];
    }
    sock_.async_write_some(boost::asio::buffer(sendBuf_, command.length()), [this_obj = this](auto &&PH1, auto &&PH2) {
        this_obj->writeCommandHandler(std::forward<decltype(PH1)>(PH1),
                                      std::forward<decltype(PH2)>(PH2));
    });
}


void BaseClient::readCommandHandler(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (err) {
        onDelete_(this);
        return;
    }

    commandHandler_(std::string(readBuf_, readBuf_ + bytes_transferred - 1), this);
    readCommand();
}


std::size_t BaseClient::checkEndOfRead(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (bytes_transferred > 0 && std::string(readBuf_ + bytes_transferred - 1 - std::strlen(END_STR), readBuf_ + bytes_transferred - 1) == std::string(END_STR)) {
        return 0;
    }
    return 1;
}


void BaseClient::writeCommandHandler(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (err) {
        onDelete_(this);
    }
}


boost::asio::ip::tcp::socket &BaseClient::getSock() {
    return sock_;
}
