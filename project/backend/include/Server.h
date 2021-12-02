#pragma once

#include <boost/asio.hpp>

#include "Connection.h"
#include "Settings.h"

class Server : public std::enable_shared_from_this<Server> {
public:
    Server(int port, std::function<void(std::shared_ptr<Connection>)> onAcceptCb,
           std::function<void(std::shared_ptr<Connection>, std::string &&)> onReadCb,
           boost::asio::io_context &service,
           std::function<void(std::shared_ptr<Connection>)> = {});

    void runServer();

    int getPort();

private:
    void startAcceptConnections();

    void handleAcceptConnection(std::shared_ptr<Connection> connection, const boost::system::error_code &err);

    boost::asio::ip::tcp::acceptor acceptor_;
    int port_;
    std::function<void(std::shared_ptr<Connection>)> onAcceptCb_;
    std::function<void(std::shared_ptr<Connection>, std::string &&)> onReadCb_;
    std::function<void(std::shared_ptr<Connection>)> onDeleteCb_;
    char readBuf_[BUFFER_LENGTH];
    char sendBuf_[BUFFER_LENGTH];
};