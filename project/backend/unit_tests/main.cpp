#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <gtest/gtest.h>
#include <iostream>


#include "Server.h"
#include "Settings.h"
#include "SharedDocument.h"

using namespace boost::asio;

#define SHARING_DOCUMENT_COMMAND "sharing document"
#define AUTH_COMMAND "auth_token="

class TestServer : public ::testing::Test {
protected:
    void SetUp() {
        myServer_ = new Server(5555);
        myServer_->runServer();
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 5555);
        sock_ = new ip::tcp::socket(service_);
        sock_->connect(ep);
    }

    void TearDown() {
        delete myServer_;
        delete sock_;
    }

    Server *myServer_;
    io_service service_;
    ip::tcp::socket *sock_;
    char buf[1024];
};

class TestSharingDocument : public ::testing::Test {
protected:
    void SetUp() {
        SharedDocument::start_since_port = 8001;
        document_ = new SharedDocument();
        document_->startShared();
        auth_token_ = document_->getAuthToken();
        port_ = document_->getPort();
    }

    void TearDown() {
        delete document_;
    }

    SharedDocument *document_;
    std::string auth_token_;
    int port_;
    io_service service_;
    char buf[1024];
};

size_t read_complete(char *buf, const std::error_code &err, size_t bytes_transferred) {
    if (bytes_transferred > 0 && std::string(buf + bytes_transferred - 1 - std::strlen(END_STR), buf + bytes_transferred - 1) == std::string(END_STR)) {
        return 0;
    }
    return 1;
}

TEST_F(TestServer, test) {
    std::strcpy(buf, SHARING_DOCUMENT_COMMAND);
    std::strcpy(buf + std::strlen(SHARING_DOCUMENT_COMMAND), END_STR);
    sock_->write_some(buffer(buf, std::strlen(SHARING_DOCUMENT_COMMAND) + std::strlen(END_STR)));

    int bytes = read(*sock_, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    std::string copy(buf, bytes - 1);
    int port = -1;
    EXPECT_NO_THROW([&]() { port = std::stoi(copy); });
    EXPECT_GE(port, 0);
    ip::tcp::socket to_shared_document_socket(service_);
    ip::tcp::endpoint shared_document_ep(ip::address::from_string("127.0.0.1"), port);
    EXPECT_NO_THROW(to_shared_document_socket.connect(shared_document_ep));
}

TEST_F(TestSharingDocument, test) {
    EXPECT_EQ(port_, 8001);

    ip::tcp::endpoint shared_doc_ep(ip::address::from_string("127.0.0.1"), port_);
    ip::tcp::socket client1(service_);
    EXPECT_NO_THROW(client1.connect(shared_doc_ep)); // подключаемся к расшаренному документы

    // аутентификация в документе
    std::strcpy(buf, AUTH_COMMAND);
    std::strcpy(buf + std::strlen(AUTH_COMMAND), (document_->getAuthToken() + END_STR).c_str());
    client1.write_some(buffer(buf, std::strlen(AUTH_COMMAND) + document_->getAuthToken().length() + std::strlen(END_STR)));

    // проверка успешности аутентификации
    int bytes = read(client1, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    EXPECT_STREQ(buf, AUTH_SUCCESS_COMMAND);

    // подключение к документу второго клиента
    ip::tcp::socket client2(service_);
    EXPECT_NO_THROW(client2.connect(shared_doc_ep));

    // аутентификация второго клиента в документе
    std::strcpy(buf, AUTH_COMMAND);
    std::strcpy(buf + std::strlen(AUTH_COMMAND), (document_->getAuthToken() + END_STR).c_str());
    client2.write_some(buffer(buf, std::strlen(AUTH_COMMAND) + document_->getAuthToken().length() + std::strlen(END_STR)));

    // проверка аутентификации
    bytes = read(client1, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    EXPECT_STREQ(buf, AUTH_SUCCESS_COMMAND);

    // отправка команды вторым клиентом
    std::string command = "aboba welcomes you";
    command += END_STR;
    std::strcpy(buf, command.c_str());
    client2.write_some(buffer(buf, command.length()));

    // проверка получение первым клиентом команды второго клиента
    bytes = read(client1, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    EXPECT_STREQ(buf, command.c_str());
}


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}