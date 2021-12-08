//
// Created by potry on 11/18/2021.
//

#include <gtest/gtest.h>

#include "JsonSerializer.h"
#include "Server.h"
#include "SharedDocumentServer.h"

#define SHARING_DOCUMENT_COMMAND "sharing document"
#define AUTH_COMMAND "auth_token="

class TestServer : public ::testing::Test {
protected:
    void SetUp() {
        myServer_ = new Server(5555, service_, ServerCallbacks{});
        myServer_->runServer();
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 5555);
        sock_ = new boost::asio::ip::tcp::socket(service_);
        sock_->connect(ep);
    }

    void TearDown() {
        delete myServer_;
        delete sock_;
    }

    Server *myServer_;
    boost::asio::io_service service_;
    boost::asio::ip::tcp::socket *sock_;
    char buf[1024];
};

int SharedDocumentServer::start_since_port = 8080;

class TestSharingDocument : public ::testing::Test {
protected:
    void SetUp() {
        document_ = new SharedDocumentServer(service_);
        document_->startShared();
        auth_token_ = document_->getAuthToken();
        port_ = document_->getPort();
    }

    void TearDown() {
        delete document_;
    }

    SharedDocumentServer *document_;
    std::string auth_token_;
    int port_;
    boost::asio::io_service service_;
    char buf[1024];
};

size_t read_complete(char *buf, const std::error_code &err, size_t bytes_transferred) {
    if (bytes_transferred > 0 && std::string(buf + bytes_transferred - 1 - std::strlen(END_STR), buf + bytes_transferred - 1) == std::string(END_STR)) {
        return 0;
    }
    return 1;
}

TEST_F(TestServer, test) {
    return;
    std::strcpy(buf, SHARING_DOCUMENT_COMMAND);
    std::strcpy(buf + std::strlen(SHARING_DOCUMENT_COMMAND), END_STR);
    sock_->write_some(boost::asio::buffer(buf, std::strlen(SHARING_DOCUMENT_COMMAND) + std::strlen(END_STR)));

    int bytes = read(*sock_, boost::asio::buffer(buf), std::bind(read_complete, buf, std::placeholders::_1, std::placeholders::_2));
    std::string copy(buf, bytes - 1);
    int port = -1;
    EXPECT_NO_THROW([&]() { port = std::stoi(copy); });
    EXPECT_GE(port, 0);
    boost::asio::ip::tcp::socket to_shared_document_socket(service_);
    boost::asio::ip::tcp::endpoint shared_document_ep(boost::asio::ip::address::from_string("127.0.0.1"), port);
    EXPECT_NO_THROW(to_shared_document_socket.connect(shared_document_ep));
}

TEST_F(TestSharingDocument, test) {
    return;
    using namespace std::placeholders;

    EXPECT_EQ(port_, 8080);

    boost::asio::ip::tcp::endpoint shared_doc_ep(boost::asio::ip::address::from_string("127.0.0.1"), port_);
    boost::asio::ip::tcp::socket client1(service_);
    EXPECT_NO_THROW(client1.connect(shared_doc_ep));// подключаемся к расшаренному документы

    // аутентификация в документе
    std::strcpy(buf, AUTH_COMMAND);
    std::strcpy(buf + std::strlen(AUTH_COMMAND), (document_->getAuthToken() + END_STR).c_str());
    client1.write_some(boost::asio::buffer(buf, std::strlen(AUTH_COMMAND) + document_->getAuthToken().length() + std::strlen(END_STR)));

    // проверка успешности аутентификации
    int bytes = read(client1, boost::asio::buffer(buf), std::bind(read_complete, buf, _1, _2));
    EXPECT_STREQ(buf, AUTH_SUCCESS_COMMAND);

    // подключение к документу второго клиента
    boost::asio::ip::tcp::socket client2(service_);
    EXPECT_NO_THROW(client2.connect(shared_doc_ep));

    // аутентификация второго клиента в документе
    std::strcpy(buf, AUTH_COMMAND);
    std::strcpy(buf + std::strlen(AUTH_COMMAND), (document_->getAuthToken() + END_STR).c_str());
    client2.write_some(boost::asio::buffer(buf, std::strlen(AUTH_COMMAND) + document_->getAuthToken().length() + std::strlen(END_STR)));

    // проверка аутентификации
    bytes = read(client1, boost::asio::buffer(buf), std::bind(read_complete, buf, _1, _2));
    EXPECT_STREQ(buf, AUTH_SUCCESS_COMMAND);

    // отправка команды вторым клиентом
    std::string command = "aboba welcomes you";
    command += END_STR;
    std::strcpy(buf, command.c_str());
    client2.write_some(boost::asio::buffer(buf, command.length()));

    // проверка получение первым клиентом команды второго клиента
    bytes = read(client1, boost::asio::buffer(buf), std::bind(read_complete, buf, _1, _2));
    EXPECT_STREQ(buf, command.c_str());
}

using SerializerType = JsonSerializer;

struct Point {
    int x, y;

    void serialize(auto &ar, const unsigned int) {
        ar &x &y;
    }

    bool operator==(const Point &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const Point &rhs) const {
        return !(rhs == *this);
    }
};

JSONCONS_ALL_MEMBER_TRAITS(Point, x, y)

TEST(serializing, serialize_simple_types) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = 1;

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    EXPECT_EQ(serializer.deserialize<decltype(value)>(stream), value);
}

TEST(serializing, serialize_complex_types) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = Point{1, 2};

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    auto result = serializer.deserialize<Point>(stream);
    EXPECT_EQ(result, value);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}