#pragma once

#include "BaseClient.h"

class DocumentClient : public BaseClient {
public:
    DocumentClient(boost::asio::io_service &service,
                   std::function<void(std::string command, BaseClient *this_client)> command_handler,
                   std::function<void(BaseClient *client)> on_delete,
                   std::function<bool(std::string &)> check_auth_token);

    void afterConnect() override;

private:
    void checkAuthToken(const boost::system::error_code &err, std::size_t bytes_transferred);

    void authSuccess();

    void authSuccessHandler(const boost::system::error_code &err, std::size_t bytes_transferred);

    void authError();

    void authErrorHandler(const boost::system::error_code &err, std::size_t bytes_transferred);


    std::function<bool(std::string &)> check_auth_token_;

    bool is_auth = false;
};