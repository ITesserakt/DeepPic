#include <iostream>
#include <boost/bind.hpp>

#include "ServerConnection.h"

ServerConnection::ServerConnection(std::string &&url, int port, ServerConnectionCallbacks &&callbacks) : socket_(service_),
                                                                                                         serverUrl_(std::move(url)),
                                                                                                         serverPort_(port),
                                                                                                         callbacks_(std::move(callbacks)),
                                                                                                         resolver_(service_) {
}

void ServerConnection::on_resolve(boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type result) {
    boost::asio::async_connect(socket_, result.begin(), result.end(),
                               std::bind(&ServerConnection::on_connect, this, std::placeholders::_1));
}

void ServerConnection::connectionToServer() {
    resolver_.async_resolve(serverUrl_, std::to_string(serverPort_), std::bind(&ServerConnection::on_resolve, this,
                                                                               std::placeholders::_1, std::placeholders::_2));
}

void ServerConnection::on_connect(boost::system::error_code err) {
    if (!err) {
        read();
    }
}

void ServerConnection::read() {
    boost::beast::http::async_read(socket_, buffer_, response_, [this](const boost::system::error_code err, size_t bytes_transferred) {
        this->readHandler(err, bytes_transferred);
    });
}

void ServerConnection::readHandler(const boost::system::error_code &err, size_t bytes_transferred) {
    if (err) {
        std::cerr << "ServerConnection::readHandler error" << std::endl;
        return;
    }

    if (callbacks_.onReadCb) {
        callbacks_.onReadCb(response_.body().data());
    }

    response_.clear();
    response_.body().clear();

    read();
}

void ServerConnection::run_ioc() {
    std::thread run_thread([this]() { service_.run(); });
    run_thread.detach();
}

void ServerConnection::start() {
    connectionToServer();
    run_ioc();
}

void ServerConnection::write(std::string &&message) {
    std::unique_lock<std::mutex> ulock(writeMutex_);
    writeCv_.wait(ulock, [this]() { return bool(canWrite_); });
    canWrite_ = false;

    request_.clear();
    request_.body().clear();

    request_.method(boost::beast::http::verb::post);
    request_.target("/");
    request_.body() = message;
    request_.prepare_payload();

    std::cout << "ServerConnection::write()" << std::endl;

    boost::beast::http::async_write(socket_, request_, [this](boost::system::error_code err, size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        this->writeHandler(err);
    });
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


void ServerConnection::setServerUrl(std::string &&url) {
    serverUrl_ = url;
}

void ServerConnection::setServerPort(int port) {
    serverPort_ = port;
}


