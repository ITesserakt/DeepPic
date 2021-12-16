#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include "Connection.h"

Connection::Connection(boost::asio::io_context &service, std::function<void(std::shared_ptr<Connection>, std::string &&)> on_read_cb,
                       std::function<void(std::shared_ptr<Connection>)> on_delete_cb) : sock_(service),
                                                                                        onReadCb_(std::move(on_read_cb)),
                                                                                        onDeleteCb_(std::move(on_delete_cb)) {

}

Connection::~Connection() {
    BOOST_LOG_TRIVIAL(info) << "~Connection()";
}

void Connection::afterConnect() {
    read();
}

void Connection::read() {
    boost::beast::http::async_read(sock_, buffer_, request_,
                                   [self = shared_from_this()](boost::system::error_code err, size_t bytes_transferred) {
                                       self->readHandler(err, bytes_transferred);
                                   });
}

void Connection::write(std::string &command, std::function<void(std::shared_ptr<Connection>)> onWriteCb) {
    // функция write может быть вызвана из разных потоков, поэтому блокируем ее до тех пор, пока другой поток не закончит запись
    std::unique_lock<std::mutex> ulock(writeMutex_);
    writeCv_.wait(ulock, [this]() { return bool(canWrite_); });
    canWrite_ = false;

    response_.clear();

    response_.keep_alive(request_.keep_alive());
    response_.version(request_.version());
    response_.result(boost::beast::http::status::ok);
    response_.body() = command;

    response_.prepare_payload();

    boost::beast::http::async_write(sock_, response_,
                                    [self = shared_from_this(), onWriteCb](boost::system::error_code err, size_t bytes_transferred) {
                                        self->writeHandler(err, bytes_transferred, onWriteCb);
                                    });
}


void Connection::readHandler(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (err) {
        BOOST_LOG_TRIVIAL(error) << "Error while read from client";
        if (onDeleteCb_) {
            onDeleteCb_(shared_from_this());
        }
        return;
    }

    onReadCb_(shared_from_this(), request_.body().data());

    request_.clear();
    request_.body().clear();

    read();
}


void Connection::writeHandler(const boost::system::error_code &err, std::size_t bytes_transferred,
                              std::function<void(std::shared_ptr<Connection>)> onWriteCb) {
    if (err) {
        if (onDeleteCb_) {
            onDeleteCb_(shared_from_this());
        }
        if (onWriteCb) {
            onWriteCb(nullptr);
        }
        return;
    }

    if (onWriteCb) {
        onWriteCb(shared_from_this());
    }

    canWrite_ = true;
    writeCv_.notify_one();
}

void Connection::stop() {
    sock_.close();
}

boost::asio::ip::tcp::socket &Connection::getSock() {
    return sock_;
}

void Connection::setAuth(bool) {
    isAuth_ = true;
}

bool Connection::getAuth() {
    return isAuth_;
}

