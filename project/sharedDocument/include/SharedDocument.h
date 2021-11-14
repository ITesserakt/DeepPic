#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <string>

#include <Client.h>

class SharedDocument {
public:
    static int start_since_port; // начиная с какого порта начнут создаваться объекты SharedDocument
    static void init_start_port(int port);

    SharedDocument();

    void startShared();

private:
    void generateAuthToken();

    void startAcceptClients();

    void handleAcceptClient(std::shared_ptr<Client> client, const boost::system::error_code &err);

    void onDeleteClient(Client *client);

    bool checkAuthToken(std::string &token);

    void sharingCommand(std::string &command, Client *author);

    void run();

    boost::asio::io_service service_;
    std::string auth_token_;
    boost::asio::ip::tcp::acceptor acceptor_;
    int port_;
    std::vector<std::shared_ptr<Client>> clients;
    //boost::log::sources::logger logger_;
};
