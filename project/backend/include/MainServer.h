#pragma once

#include "Server.h"
#include "Connection.h"
#include "IManageCommand.h"

class MainServer {
public:
    MainServer(int port, int countThreads);
    void runServer();

private:
    void handleAcceptConnection(std::shared_ptr<Connection> connection);

    void onReadCb(std::shared_ptr<Connection>, std::string &&command);

    boost::asio::io_context service_;
    Server server_;
    IManageCommand *createNewDocumentCommand_;
    int countThreads_;
};