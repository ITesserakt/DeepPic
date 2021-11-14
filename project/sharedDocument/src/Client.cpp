#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>
#include <utility>

#include "Client.h"


Client::Client(boost::asio::io_service &service,
               std::function<void(std::string command, Client *this_client)> command_handler,
               std::function<bool(std::string &)> check_token,
               std::function<void(Client *client)> on_delete) : sock_(service),
                                                                command_handler_(std::move(command_handler)),
                                                                check_auth_token_(std::move(check_token)),
                                                                on_delete_(std::move(on_delete)) {
}

void Client::readAuthToken() {
    boost::asio::async_read(sock_, boost::asio::buffer(read_buf_),
                            boost::bind(&Client::end_of_token, shared_from_this(), boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred),
                            boost::bind(&Client::checkAuthToken, shared_from_this(), boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    std::cerr << "client sock addr: " << sock_.remote_endpoint().address().to_string() << std::endl;
}


void Client::checkAuthToken(const boost::system::error_code &e, std::size_t bytes_transferred) {
    if (e) {
        on_delete_(this);
        return;
    }
    std::cerr << "check auth token" << std::endl;
    std::cerr << read_buf_ << " " << bytes_transferred << std::endl;
    std::string auth_token(read_buf_, read_buf_ + bytes_transferred - 1);
    if (check_auth_token_(auth_token)) {
        authSuccess();
    } else {

        on_delete_(this);
        // TODO: возможно сделать какую-нибудь отправку о неуспешной авторизации
        return;
    }
}

void Client::authSuccess() {
    strcpy(send_buf_, "auth success");
    std::cerr << "auth success" << std::endl;
    sock_.async_write_some(boost::asio::buffer(send_buf_, strlen("auth success")),
                           [this_obj = shared_from_this()](auto &&PH1, auto &&PH2) {
                               this_obj->startReadCommand(std::forward<decltype(PH1)>(PH1),
                                                          std::forward<decltype(PH2)>(PH2));
                           });
}

void Client::startReadCommand(const boost::system::error_code &e, std::size_t bytes_transferred) {
    if (e) {
        on_delete_(this);
        return;
    }
    readCommand();
}

void Client::readCommand() {
    sock_.async_read_some(boost::asio::buffer(read_buf_), [this_obj = shared_from_this()](auto &&PH1, auto &&PH2) {
        this_obj->notifyOtherClients(std::forward<decltype(PH1)>(PH1),
                                     std::forward<decltype(PH2)>(PH2));
    });
}

void Client::notifyOtherClients(const boost::system::error_code &e, std::size_t bytes_transferred) {
    if (e) {
        on_delete_(this);
        return;
    }
    command_handler_(read_buf_, this);
    readCommand();
}

void Client::writeCommand(std::string &command) {
    // TODO: сделать так, чтобы единовременно могла отправляться только одна команда. мб как-то через мьютекс
    for (int i = 0; i < command.length(); ++i) {
        send_buf_[i] = command[i];
    }
    sock_.async_write_some(boost::asio::buffer(send_buf_, command.length()), [this_obj = shared_from_this()](auto &&PH1, auto &&PH2) {
        this_obj->startReadCommand(std::forward<decltype(PH1)>(PH1),
                                   std::forward<decltype(PH2)>(PH2));
    });
}

size_t Client::end_of_token(const boost::system::error_code &err, size_t bytes) {
    std::cout << read_buf_ << std::endl;
    if (bytes > 0 && read_buf_[bytes - 1] == '\n') {
        return 0;
    }
    return 1;
}
