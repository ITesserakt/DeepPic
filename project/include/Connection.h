#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "Settings.h"

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(boost::asio::io_context &service,
               std::function<void(std::shared_ptr<Connection>, std::string &&)> on_read_cb,
               std::function<void(std::shared_ptr<Connection>)> on_delete_cb = {});

    boost::asio::ip::tcp::socket &getSock();

    void afterConnect();

    void write(std::string &command, std::function<void(std::shared_ptr<Connection>)> onWriteCb = {});

    void stop();

    void setAuth(bool value);

    bool getAuth();

    ~Connection();

private:
    void read();

    void readHandler(const boost::system::error_code &err, std::size_t bytes_transferred);

    std::size_t checkEndOfRead(const boost::system::error_code &err, std::size_t bytes_transferred);

    void writeHandler(const boost::system::error_code &err, std::size_t bytes_transferred,
                      std::function<void(std::shared_ptr<Connection>)> onWriteCb);

    boost::asio::ip::tcp::socket sock_;

    std::function<void(std::shared_ptr<Connection>, std::string &&)> onReadCb_;
    std::function<void(std::shared_ptr<Connection>)> onDeleteCb_;

    boost::beast::flat_buffer buffer_{BUFFER_LENGTH};

    boost::beast::http::request<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::string_body> response_;

    std::mutex writeMutex_;
    std::condition_variable writeCv_;
    std::atomic<bool> canWrite_ = true;

    bool isAuth_ = false;
};