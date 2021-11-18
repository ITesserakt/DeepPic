#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "DocumentClient.h"
#include "Settings.h"

class SharedDocument {
public:
    static int start_since_port;// начиная с какого порта начнут создаваться объекты SharedDocument

    SharedDocument();

    void startShared();

    int getPort();

    std::string getAuthToken();

private:
    void generateAuthToken();

    void startAcceptClients();

    void handleAcceptClient(std::shared_ptr<DocumentClient> client, const boost::system::error_code &err);

    void deleteClient(BaseClient *client);

    bool checkAuthToken(std::string &token);

    void sharingCommand(std::string &command, BaseClient *author);

    void run();

    void getDocumentFromClient();

    void handleGetDocumentFromClient(const boost::system::error_code &err, std::size_t bytes_transferred);

    void sendDocumentToNewClients();

    boost::asio::io_service service_;
    std::string auth_token_;
    boost::asio::ip::tcp::acceptor acceptor_;
    int port_;
    std::vector<std::shared_ptr<DocumentClient>> clients_;
    std::vector<std::shared_ptr<DocumentClient>> toGetDocument_;
    char document_[MAX_DOCUMENT_LENGTH];
};
