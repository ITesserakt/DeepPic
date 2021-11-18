#include <boost/asio.hpp>

#include "BaseClient.h"
#include "Settings.h"

class Server {
public:
    Server(int port);

    void runServer();

private:
    void startAcceptClients();
    void handleAcceptClient(std::shared_ptr<BaseClient> client, boost::system::error_code &err);
    void writeDocumentPortToClient();
    void handleWriteDocumentPort(const boost::system::error_code &err, std::size_t bytes_transferred);

    boost::asio::io_service service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    int port_;
    char read_buf_[BUFFER_LENGTH];
    char send_buf_[BUFFER_LENGTH];
};