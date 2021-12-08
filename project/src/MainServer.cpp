#include "MainServer.h"
#include "IManageCommand.h"

#include <utility>
#include <thread>

MainServer::MainServer(int port, int countThreads) : service_(), server_(port, service_,
                                                                         ServerCallbacks{
                                                                                 [this](std::shared_ptr<Connection> connection) {
                                                                                     this->handleAcceptConnection(std::move(connection));
                                                                                 },
                                                                                 [this](std::shared_ptr<Connection> connection,
                                                                                        std::string &&command) {
                                                                                     this->onReadCb(std::move(connection),
                                                                                                    std::move(command));
                                                                                 }, {}}
),
                                                     countThreads_(countThreads) {
    createNewDocumentCommand_ = new IManageCommand(CREATE_DOCUMENT);
}

void MainServer::runServer() {
    server_.runServer();

    std::vector<std::thread> threads;
    threads.reserve(countThreads_);
    for (int i = 0; i < countThreads_; ++i) {
        threads.emplace_back([this]() {
            service_.run();
        });
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

void MainServer::handleAcceptConnection(std::shared_ptr<Connection> connection) {
    connection->afterConnect();
}

void MainServer::onReadCb(std::shared_ptr<Connection> connection, std::string &&command) {
    createNewDocumentCommand_->do_command(command, std::move(connection));
}
