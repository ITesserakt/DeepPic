#pragma once

#include <boost/asio.hpp>
#include <functional>

#define BUFFERS_RESERVE 1024

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(boost::asio::io_service &service,
           std::function<void(std::string command, Client *this_client)> command_handler,
           std::function<bool(std::string &)> check_token, std::function<void(Client *client)> on_delete);

    void readAuthToken(); // вызывается при подключении клиента. вызывает функцию асинхронного чтения

    // вызовется после чтения токена. если все пройдет успешно, отправит клиенту сообщение об успехе, иначе об ошибки
    void checkAuthToken(const boost::system::error_code &e, std::size_t bytes_transferred);

    size_t end_of_token(const boost::system::error_code &, size_t bytes);

    // отправляет клиенту сообщение об успешной аутентификации и handler'ом делает read_command()
    void authSuccess();

    void startReadCommand(const boost::system::error_code &e,
                          std::size_t bytes_transferred);

    void readCommand(); // вызывает асинхронное чтение команд

    // вызовется после получения команды. отправит документу уведомление что нужно отправить команду остальным клиентам.
    void notifyOtherClients(const boost::system::error_code &e, std::size_t bytes_transferred);

    // вызовется классом SharedDocument, если вдруг придут команды от других пользователей
    // TODO: на время вызова функции до того момента, пока не отправится одна команда, заблокировать эту функцию
    // иначе перезаатрется send_buf_. Возможно с помощью мьютекса
    void writeCommand(std::string &command);

    boost::asio::ip::tcp::socket &sock() { return sock_; }

private:
    boost::asio::ip::tcp::socket sock_;
    std::function<void(std::string command, Client *this_client)> command_handler_;
    std::function<bool(std::string &)> check_auth_token_;
    std::function<void(Client *client)> on_delete_;
    char read_buf_[BUFFERS_RESERVE];
    char send_buf_[BUFFERS_RESERVE];
};