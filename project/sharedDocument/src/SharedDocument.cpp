#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

#include "SharedDocument.h"
#include "Client.h"
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>

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
    std::shared_ptr<Client> client(new Client(service_,
                                              [this](std::string command, Client *author) {
                                                  return this->sharingCommand(command, author);
                                              },
                                              [this](std::string &token) { return this->checkAuthToken(token); },
                                              [this](Client *client) { return this->onDeleteClient(client); }));
    clients.push_back(client);

    acceptor_.async_accept(client->sock(), boost::bind(&SharedDocument::handleAcceptClient, this, client,
                                                       boost::asio::placeholders::error));
}

void SharedDocument::sharingCommand(std::string &command, Client *author) {
    for (auto &client: clients) {
        if (&(*client) != author) {
            client->writeCommand(command);
        }
    }
}

void SharedDocument::onDeleteClient(Client *client) {
    auto pos = clients.begin();
    for (auto &cl: clients) {
        if (&(*cl) == client) {
            break;
        }
        pos++;
    }
    try {
        std::cerr << "delete client" << std::endl;
        clients.erase(pos);
    }
    catch (...) {

    }
}

void SharedDocument::handleAcceptClient(std::shared_ptr<Client> client, const boost::system::error_code &err) {
    if (err) {
        onDeleteClient(&(*client));
        std::cerr << "Error while accept client" << std::endl;
        return;
    }

    std::cerr << "Accept client" << std::endl;
    client->readAuthToken();
    startAcceptClients();
}

void SharedDocument::init_start_port(int port) {
    SharedDocument::start_since_port = port;
}

void SharedDocument::run() {
    service_.run();
}
