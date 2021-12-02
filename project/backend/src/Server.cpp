#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <utility>

#include "Server.h"


Server::Server(int port, std::function<void(std::shared_ptr<Connection>)> onAcceptCb,
               std::function<void(std::shared_ptr<Connection>, std::string &&)> onReadCb,
               boost::asio::io_context &service,
               std::function<void(std::shared_ptr<Connection>)> onDeleteCb) : acceptor_(service),
                                                                              port_(port),
                                                                              onAcceptCb_(std::move(
                                                                                      onAcceptCb)),
                                                                              onReadCb_(std::move(
                                                                                      onReadCb)),
                                                                              onDeleteCb_(std::move(onDeleteCb)) {

}

void Server::runServer() {
    std::cerr << "runServer()" << std::endl;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen(1024);

    startAcceptConnections();
}

int Server::getPort() {
    return port_;
}

void Server::startAcceptConnections() {
    std::shared_ptr<Connection> connection(
            new Connection(static_cast<boost::asio::io_context &>(acceptor_.get_executor().context()), onReadCb_));
    acceptor_.async_accept(connection->getSock(), boost::bind(&Server::handleAcceptConnection, this,
                                                              connection, boost::asio::placeholders::error));
    std::cerr << "start accept connections" << std::endl;
}

void Server::handleAcceptConnection(std::shared_ptr<Connection> connection, const boost::system::error_code &err) {
    if (err) {
        startAcceptConnections();
    }

    onAcceptCb_(connection);
    startAcceptConnections();
}

