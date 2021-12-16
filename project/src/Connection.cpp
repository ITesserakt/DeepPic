#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "Connection.h"

Connection::Connection(boost::asio::io_context &service, std::function<void(std::shared_ptr<Connection>, std::string &&)> on_read_cb,
                       std::function<void(std::shared_ptr<Connection>)> on_delete_cb) : sock_(service),
                                                                                        onReadCb_(std::move(on_read_cb)),
                                                                                        onDeleteCb_(std::move(on_delete_cb)) {

}

Connection::~Connection() {
    std::cout << "~Connection()" << std::endl;
}

void Connection::afterConnect() {
    read();
}

void Connection::read() {
    std::cerr << "we are in Connection::read" << std::endl;
    boost::asio::async_read(sock_, boost::asio::buffer(readBuf_),
                            boost::bind(&Connection::checkEndOfRead, shared_from_this(), boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred),
                            boost::bind(&Connection::readHandler, shared_from_this(), boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void Connection::write(std::string &command, std::function<void(std::shared_ptr<Connection>)> onWriteCb) {
    // функция write может быть вызвана из разных потоков, поэтому блокируем ее до тех пор, пока другой поток не закончит запись
    std::unique_lock<std::mutex> ulock(writeMutex_);
    writeCv_.wait(ulock, [this]() { return bool(canWrite_); });
    canWrite_ = false;

    for (int i = 0; i < command.length(); ++i) {
        sendBuf_[i] = command[i];
    }
    std::string end_str(END_STR);
    for (int i = command.length(); i < command.length() + end_str.length(); ++i) {
        sendBuf_[i] = end_str[i - command.length()];
    }

    sock_.async_write_some(boost::asio::buffer(sendBuf_, command.length() + end_str.length()),
                           boost::bind(&Connection::writeHandler, shared_from_this(), _1, _2, onWriteCb));
}


void Connection::readHandler(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (err) {
        std::cerr << "Connection::readHandler error" << std::endl;
        if (onDeleteCb_) {
            onDeleteCb_(shared_from_this());
        }
        return;
    }

    onReadCb_(shared_from_this(), std::string(readBuf_, readBuf_ + bytes_transferred));
    memset(readBuf_, 0, bytes_transferred);
    read();
}


std::size_t Connection::checkEndOfRead(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (bytes_transferred > 2 &&
        readBuf_[bytes_transferred - 1] == '\n' && readBuf_[bytes_transferred - 2] == '\r')
        //std::string(readBuf_ + bytes_transferred - 1 - std::strlen(END_STR), readBuf_ + bytes_transferred - 1) == std::string(END_STR))
    {
        return 0;
    }
    return 1;
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
    std::cerr << "we are on Connection::writeHandler" << std::endl;

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

