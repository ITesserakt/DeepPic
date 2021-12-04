#pragma once

#include <boost/asio.hpp>

#include "Connection.h"
#include "Settings.h"

struct ServerCallbacks {
    std::function<void(std::shared_ptr<Connection>)> onAcceptCb_;
    std::function<void(std::shared_ptr<Connection>, std::string &&)> onReadCb_;
    std::function<void(std::shared_ptr<Connection>)> onDeleteCb_;
};

class Server {
public:
    Server(int port, boost::asio::io_context &service, ServerCallbacks &&callbacks);

    void runServer();

    int getPort();

private:
    void startAcceptConnections();

    void handleAcceptConnection(std::shared_ptr<Connection> connection, const boost::system::error_code &err);

    boost::asio::ip::tcp::acceptor acceptor_;
    int port_;

    ServerCallbacks callbacks_;

    char readBuf_[BUFFER_LENGTH];
    char sendBuf_[BUFFER_LENGTH];
};