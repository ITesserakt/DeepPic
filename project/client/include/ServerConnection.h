#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <functional>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

#define BUFFER_LENGTH 6000000
#define END_STR "\r\n"

#define SERVER_PORT 8080


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

    void setServerUrl(std::string &&url);

    void setServerPort(int port);

    std::string getServerUrl() {
        return serverUrl_;
    }

    int getServerPort() {
        return serverPort_;
    };

    void stop() {
        socket_.close();
    }

private:
    void read();

    void connectionToServer();

    void readHandler(const boost::system::error_code &err, size_t bytes_transferred);

    void writeHandler(const boost::system::error_code &err);

    void run_ioc();

    std::string serverUrl_;
    int serverPort_;

    boost::asio::io_context service_;
    boost::asio::ip::tcp::socket socket_;
    ServerConnectionCallbacks callbacks_;

    std::mutex writeMutex_;
    std::condition_variable writeCv_;
    std::atomic<bool> canWrite_{true};

    boost::beast::flat_buffer buffer_{BUFFER_LENGTH};
    boost::beast::http::request<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::string_body> response_;
    boost::asio::ip::tcp::resolver resolver_;

};

