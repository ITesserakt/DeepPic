#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "SharedDocument.h"
#include <boost/log/common.hpp>

SharedDocument::SharedDocument() : acceptor_(service_), port_(start_since_port++) {
    generateAuthToken();
}

void SharedDocument::generateAuthToken() {
    auth_token_ = "dummy_aboba";
}

void SharedDocument::startShared() {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen(1024);
    startAcceptClients();

    std::cerr << "Start shared on port " << port_ << std::endl;

    service_.run();
    /*
    std::vector<std::thread> threads;
    threads.reserve(4);
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([this] { run(); });
    }

    for (auto &thread : threads) {
        thread.join();
    }
     */
}


bool SharedDocument::checkAuthToken(std::string &token) {
    return token == auth_token_;
}


void SharedDocument::startAcceptClients() {
    std::shared_ptr<DocumentClient> client(new DocumentClient(
            service_,
            [this](std::string command, BaseClient *author) {
                return this->sharingCommand(command, author);
            },
            [this](BaseClient *client) { return this->deleteClient(client); },
            [this](std::string &token) { return this->checkAuthToken(token); }));
    clients_.push_back(client);

    acceptor_.async_accept(client->getSock(), boost::bind(&SharedDocument::handleAcceptClient, this, client,
                                                          boost::asio::placeholders::error));
}


void SharedDocument::sharingCommand(std::string &command, BaseClient *author) {
    for (auto &client : clients_) {
        if (&(*client) != author) {
            client->writeCommand(command);
        }
    }
}


void SharedDocument::deleteClient(BaseClient *client) {
    auto pos = clients_.begin();
    for (auto &cl : clients_) {
        if (&(*cl) == client) {
            break;
        }
        pos++;
    }
    try {
        std::cerr << "delete client" << std::endl;
        clients_.erase(pos);
    } catch (...) {
    }
}


void SharedDocument::handleAcceptClient(std::shared_ptr<DocumentClient> client, const boost::system::error_code &err) {
    if (err) {
        deleteClient(&(*client));
        std::cerr << "Error while accept client" << std::endl;
        return;
    }

    std::cerr << "Accept client" << std::endl;
    client->afterConnect();
    startAcceptClients();
}


void SharedDocument::run() {
    service_.run();
}


int SharedDocument::getPort() {
    return port_;
}


std::string SharedDocument::getAuthToken() {
    return auth_token_;
}
