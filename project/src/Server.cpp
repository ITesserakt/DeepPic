#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <utility>

#include "Server.h"


Server::Server(int port, boost::asio::io_context &service, ServerCallbacks &&callbacks) : acceptor_(service),
                                                                                          port_(port),
                                                                                          callbacks_(std::move(callbacks)) {

}

void Server::runServer() {
    BOOST_LOG_TRIVIAL(info) << "Server run";
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
            new Connection(static_cast<boost::asio::io_context &>(acceptor_.get_executor().context()), callbacks_.onReadCb_, callbacks_.onDeleteCb_));
    acceptor_.async_accept(connection->getSock(), boost::bind(&Server::handleAcceptConnection, this,
                                                              connection, boost::asio::placeholders::error));
    BOOST_LOG_TRIVIAL(info) << "Start accept connections";
}

void Server::handleAcceptConnection(std::shared_ptr<Connection> connection, const boost::system::error_code &err) {
    if (err) {
        startAcceptConnections();
    }

    callbacks_.onAcceptCb_(connection);
    startAcceptConnections();
}



