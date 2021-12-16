#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "SharedDocumentServer.h"
#include "Command.h"
#include "CommandConstructor.h"
#include <boost/log/trivial.hpp>

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
    static std::vector<char> syms = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                     'u', 'v', 'w', 'x', 'y', 'z', '_', '-', '$', '@'};
    static int token_length = 20;
    for (int i = 0; i < token_length; ++i) {
        authToken_ += syms[rand() % syms.size()];
    }
}

void SharedDocumentServer::startShared() {
    server_.runServer();
}


bool SharedDocumentServer::checkAuthToken(std::string &&token) {
    return token == authToken_;
}


void SharedDocumentServer::onReadCb(std::shared_ptr<Connection> author, std::string &&command) {
    BOOST_LOG_TRIVIAL(info) << "command from user: " << command;
    //std::string command_cpy = command;
    //documentCommandBus_->do_command(std::move(command_cpy), author);
    auto command_parse = json::parse(command);
    if (command_parse["target"] == "auth" && !author->getAuth()) {
        if (checkAuthToken(command_parse["auth_token"])) {
            std::string response_dump = CommandConstructor::authServer("OK", getAuthToken(), std::string("127.0.0.1"), getPort()).dump();
            author->setAuth(true);
            author->write(response_dump);
            BOOST_LOG_TRIVIAL(info) << "Connect new client to document";
        } else {
            std::string response_dump = CommandConstructor::authServer("FAIL", "", std::string("127.0.0.1"), getPort()).dump();
            author->setAuth(false);
            // если пользователь послал не верный аутентификационный токен - удаляем его
            author->write(response_dump, [](std::shared_ptr<Connection> connection) { connection->stop(); });
        }
    } else if (author->getAuth()) {
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
        BOOST_LOG_TRIVIAL(info) << "Delete client connection";
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
    BOOST_LOG_TRIVIAL(info) << "Accept new client to document";
    connections_.push_back(connection);
    connection->afterConnect();
}
