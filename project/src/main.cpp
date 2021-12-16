#include <iostream>

#include "SharedDocumentServer.h"
#include "MainServer.h"

int SharedDocumentServer::start_since_port = 6070;

int main() {
    srand(time(NULL));
    MainServer server(8080, 4);
    server.runServer();
}