#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

#define BUFFER_LENGTH 1024
#define END_STR "\r\n"


/*
 * Структура с колбеками, которые будут вызываться после успешного чтения и записи соответственно.
 */
struct ServerConnectionCallbacks {
    std::function<void(std::string &&)> onReadCb;
    std::function<void(const boost::system::error_code &err)> onWriteCb;
};

class ServerConnection {
public:
    ServerConnection(std::string &&url, int port, ServerConnectionCallbacks &&callbacks);

    /*
     * Выполняет подключение к серверу и начинает бесконечный цикл чтения сообщений с сервера.
     * После того как прочитали сообщение, вызовется onReadCb из ServerConnectionCallbacks.
     */
    void start();

    /**
     * Пишет серверу какое-либо сообщение. После записи вызовет onWriteCb, переданный в структуре ServerConnectionCallbacks
     * @param message само сообщение
     */
    void write(std::string &&message);

private:
    void connectionToServer();

    void read();

    void readHandler(boost::system::error_code &err, size_t bytes_transferred);

    std::size_t checkEndOfRead(const boost::system::error_code &err, std::size_t bytes_transferred);

    void writeHandler(boost::system::error_code &err);

    void run_ioc();

    std::string serverUrl_;
    int serverPort_;

    boost::asio::io_context service_;
    boost::asio::ip::tcp::socket socket_;
    ServerConnectionCallbacks callbacks_;

    std::mutex writeMutex_;
    std::condition_variable writeCv_;
    std::atomic<bool> canWrite_ = true;

    char readBuf_[BUFFER_LENGTH];
    char sendBuf_[BUFFER_LENGTH];
};

