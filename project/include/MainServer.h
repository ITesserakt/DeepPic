#pragma once

#include "Server.h"
#include "Connection.h"
#include "Command.h"

class MainServer {
public:
    MainServer(int port, int countThreads);
    void runServer();

private:
    void handleAcceptConnection(std::shared_ptr<Connection> connection);

    void onReadCb(std::shared_ptr<Connection>, std::string &&command);

    boost::asio::io_context service_;
    Server server_;
    Command *createNewDocumentCommand_;
    int countThreads_;
};