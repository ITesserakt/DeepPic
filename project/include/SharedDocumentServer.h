#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "Server.h"
#include "Connection.h"
#include "Settings.h"

class DocumentCommandBus;

class SharedDocumentServer {
public:
    static int start_since_port; // начиная с какого порта начнут создаваться объекты SharedDocumentServer

    SharedDocumentServer(boost::asio::io_context &service);

    void startShared();

    int getPort();

    std::string getAuthToken();

    ~SharedDocumentServer();

private:
    void generateAuthToken();

    void handleAcceptConnection(std::shared_ptr<Connection> connection);

    void onDeleteConnection(std::shared_ptr<Connection> connection);

    bool checkAuthToken(std::string &token);

    void onReadCb(std::shared_ptr<Connection> author, std::string &&command);


    Server server_;
    std::string authToken_;
    std::vector<std::shared_ptr<Connection>> connections_;

    DocumentCommandBus *documentCommandBus_;
};
