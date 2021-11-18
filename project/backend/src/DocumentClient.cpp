#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <utility>

#include "DocumentClient.h"


void DocumentClient::afterConnect() {
    boost::asio::async_read(sock_, boost::asio::buffer(read_buf_),
                            boost::bind(&DocumentClient::checkEndOfRead, this, boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred),
                            boost::bind(&DocumentClient::checkAuthToken, this, boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}


DocumentClient::DocumentClient(boost::asio::io_service &service, std::function<void(std::string, BaseClient *)> command_handler,
                               std::function<void(BaseClient *)> on_delete,
                               std::function<bool(std::string &)> check_auth_token) : BaseClient(service, std::move(command_handler), std::move(on_delete)),
                                                                                      check_auth_token_(std::move(check_auth_token)) {
}


void DocumentClient::checkAuthToken(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (err) {
        on_delete_(this);
        return;
    }
    std::string auth_token(read_buf_, read_buf_ + bytes_transferred - 1);
    if (check_auth_token_(auth_token)) {
        authSuccess();
    } else {

        on_delete_(this);
        // TODO: возможно сделать какую-нибудь отправку о неуспешной авторизации
        return;
    }
}


void DocumentClient::authSuccess() {
    std::strcpy(send_buf_, "auth success");
    sock_.async_write_some(boost::asio::buffer(send_buf_, strlen("auth success")),
                           [this_obj = this](auto &&PH1, auto &&PH2) {
                               this_obj->authSuccessHandler(PH1, PH2);
                           });
}


void DocumentClient::authSuccessHandler(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if (err) {
        on_delete_(this);
        return;
    }
    is_auth = true;

    readCommand();
}


void DocumentClient::authError() {
    std::strcpy(send_buf_, "auth error");
    sock_.async_write_some(boost::asio::buffer(send_buf_, strlen("auth error")),
                           [this_obj = this](auto &&PH1, auto &&PH2) {
                               this_obj->authErrorHandler(PH1, PH2);
                           });
}


void DocumentClient::authErrorHandler(const boost::system::error_code &err, std::size_t bytes_transferred) {
    on_delete_(this);
}
