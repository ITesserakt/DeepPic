#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "SharedDocumentServer.h"
#include "IManageCommand.h"
#include "CommandConstructor.h"
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
    documentCommandBus_ = new DocumentCommandBus(&connections_);
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
    std::cout << command << std::endl;
    //std::string command_cpy = command;
    //documentCommandBus_->do_command(std::move(command_cpy), author);
    auto command_parse = json::parse(command);
    if (command_parse["target"] == "auth" && !author->getAuth()) {
        if (command_parse["auth_token"] == getAuthToken()) {
            std::string response_dump = CommandConstructor::authServer("OK", getAuthToken(), std::string("127.0.0.1"), getPort()).dump();
            author->setAuth(true);
            author->write(response_dump);
        } else {
            std::string response_dump = CommandConstructor::authServer("FAIL", getAuthToken(), std::string("127.0.0.1"), getPort()).dump();
            author->setAuth(false);
            // если пользователь послал не верный аутентификационный токен - удаляем его
            author->write(response_dump, [](std::shared_ptr<Connection> connection) { connection->stop(); });
        }
    } else {
        documentCommandBus_->do_command(std::move(command), author);
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
        std::cerr << "delete ServerConnection" << std::endl;
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
    delete documentCommandBus_;
}

void SharedDocumentServer::handleAcceptConnection(std::shared_ptr<Connection> connection) {
    std::cerr << "we are in SharedDocumentServer::handleAcceptConnection" << std::endl;
    connections_.push_back(connection);
    connection->afterConnect();
}
