#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "SharedDocumentServer.h"
#include "IManageCommand.h"
#include <boost/log/common.hpp>

SharedDocumentServer::SharedDocumentServer(boost::asio::io_context &service) : server_(start_since_port++, service,
                                                                                       ServerCallbacks{
                                                                                               [this](std::shared_ptr<Connection> connection) {
                                                                                                   this->handleAcceptConnection(
                                                                                                           std::move(connection));
                                                                                               },
                                                                                               [this](std::shared_ptr<Connection> author,
                                                                                                      std::string &&command) {
                                                                                                   this->onReadCb(std::move(author),
                                                                                                                  std::move(command));
                                                                                               },
                                                                                               [this](std::shared_ptr<Connection> connection) {
                                                                                                   this->onDeleteConnection(
                                                                                                           std::move(connection));
                                                                                               }}) {

    generateAuthToken();
    sharingCommand_ = new IManageCommand(SHARING_COMMAND, &connections_);
    getDocument_ = new IManageCommand(GET_DOCUMENT, &connections_);
}

void SharedDocumentServer::generateAuthToken() {
    authToken_ = "dummy_aboba";
}

void SharedDocumentServer::startShared() {
    std::cerr << "startShared()" << std::endl;
    server_.runServer();
}


bool SharedDocumentServer::checkAuthToken(std::string &token) {
    return token == authToken_;
}


void SharedDocumentServer::onReadCb(std::shared_ptr<Connection> author, std::string &&command) {
    std::cout << "We are in onReadCb" << std::endl;
    // TODO: тоже стоят заглушки вместо парсинга команд. Переделать на парсинг json'а
    if (command == "sharind command") {
        sharingCommand_->do_command(command, author);
    } else if (command == "get document" || command == "document received") {
        getDocument_->do_command(command, author);
    }
}


void SharedDocumentServer::onDeleteConnection(std::shared_ptr<Connection> connection) {
    auto pos = connections_.begin();
    for (auto &cl: connections_) {
        if (*pos == connection) {
            break;
        }
        pos++;
    }
    try {
        std::cerr << "delete client" << std::endl;
        (*pos)->stop();
        connections_.erase(pos);
    } catch (...) {
    }
}

int SharedDocumentServer::getPort() {
    return server_.getPort();
}


std::string SharedDocumentServer::getAuthToken() {
    return authToken_;
}

SharedDocumentServer::~SharedDocumentServer() {
    delete getDocument_;
    delete sharingCommand_;
}

void SharedDocumentServer::handleAcceptConnection(std::shared_ptr<Connection> connection) {
    std::cerr << "we are in SharedDocumentServer::handleAcceptConnection" << std::endl;
    connections_.push_back(connection);
    connection->afterConnect();
}
