#include <iostream>
#include <boost/bind.hpp>

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string &&url, int port, ServerConnectionCallbacks &&callbacks) : socket_(service_),
                                                                                                         serverUrl_(std::move(url)),
                                                                                                         serverPort_(port),
                                                                                                         callbacks_(std::move(callbacks)) {
}

void ServerConnection::connectionToServer() {
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(serverUrl_), serverPort_);
    socket_.connect(ep);
    read();
}

void ServerConnection::read() {
    boost::asio::async_read(socket_, boost::asio::buffer(readBuf_),
                            [this](const boost::system::error_code &err, std::size_t bytes_transferred) -> size_t {
                                return this->checkEndOfRead(err, bytes_transferred);
                            }, [this](const boost::system::error_code &err, std::size_t bytes_transferred) {
                this->readHandler(err, bytes_transferred);
            });

}

void ServerConnection::readHandler(const boost::system::error_code &err, size_t bytes_transferred) {
    if (err) {
        std::cerr << "ServerConnection::readHandler error" << std::endl;
        return;
    }

    if (callbacks_.onReadCb) {
        callbacks_.onReadCb(std::string(readBuf_, readBuf_ + bytes_transferred));
    }

    read();
}

void ServerConnection::run_ioc() {
    service_.run();
}

void ServerConnection::start() {
    connectionToServer();
    run_ioc();
}

void ServerConnection::write(std::string &&message) {
    std::unique_lock<std::mutex> ulock(writeMutex_);
    writeCv_.wait(ulock, [this]() { return bool(canWrite_); });
    canWrite_ = false;

    for (int i = 0; i < message.length(); ++i) {
        sendBuf_[i] = message[i];
    }

    std::cout << "ServerConnection::write()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    boost::asio::async_write(socket_, boost::asio::buffer(sendBuf_, message.length()),
                             [this](const boost::system::error_code &err, size_t bytes_transferred) { this->writeHandler(err); });
}

void ServerConnection::writeHandler(const boost::system::error_code &err) {
    if (err) {
        std::cerr << "ServerConnection::writeHandler error = " << err.message() << std::endl;

        return;
    }

    if (callbacks_.onWriteCb) {
        callbacks_.onWriteCb(err);
    }

    std::cout << "ServerConnection::writeHandler()" << std::endl;
    canWrite_ = true;
    writeCv_.notify_one();
}

std::size_t ServerConnection::checkEndOfRead(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (bytes_transferred > 2 &&
        readBuf_[bytes_transferred - 1] == '\n' && readBuf_[bytes_transferred - 2] == '\r')
        //std::string(readBuf_ + bytes_transferred - 1 - std::strlen(END_STR), readBuf_ + bytes_transferred - 1) == std::string(END_STR))
    {
        return 0;
    }
    return 1;
}

void ServerConnection::setServerUrl(std::string &&url) {
    serverUrl_ = url;
}

void ServerConnection::setServerPort(int port) {
    serverPort_ = port;
}

void ServerConnection::reconnectionToServer() {
    socket_.close();
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(serverUrl_), serverPort_);
    socket_.connect(ep);
    read();
}
