#include <iostream>

#include "IManageCommand.h"

IManageCommand::IManageCommand(command_t type_command) {
    switch (type_command) {
        case GET_DOCUMENT:
            letter_ = new GetDocument();
        case SHARING_COMMAND:
            letter_ = new SharingCommand();
        case CREATE_DOCUMENT:
            letter_ = new CreateNewDocumentCommand();
    }
}

IManageCommand::IManageCommand(command_t type_command, std::vector<std::shared_ptr<Connection>> *connection) : IManageCommand(
        type_command) {
    connections_ = connection;
}

bool IManageCommand::do_command(std::string &command, std::shared_ptr<Connection> author) {
    letter_->do_command(command, author);

    return true;
}

IManageCommand::~IManageCommand() {
    delete letter_;
}

bool GetDocument::do_command(std::string &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in get document" << std::endl;
    // TODO: здесь сейчас вместо реального парсинга команды заглушки. реальная команда будет приходить в json'е, поэтому поменять
    //  потом на него
    if (command == "get document") {
        clientsToGetDocument_.push_back(author);
        getDocumentFromClient();
    } else if (command == "document received") {
        sendDocumentToNewClients(std::move(command));
    }

    return true;
}

void GetDocument::getDocumentFromClient() {
    std::string give_me_document = "give me document";
    (*(connections_->begin()))->write(give_me_document,
                                      [this](std::shared_ptr<Connection> connection) { this->handleGetDocumentFromClient(connection); });
}

void GetDocument::handleGetDocumentFromClient(std::shared_ptr<Connection> &connection) {
    if (connection == nullptr) {
        getDocumentFromClient();
    }
}

void GetDocument::sendDocumentToNewClients(std::string &&document) {
    for (auto &client : clientsToGetDocument_) {
        client->write(document);
    }
}

bool SharingCommand::do_command(std::string &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in sharing command" << std::endl;
    for (auto &connection: *connections_) {
        if (author != connection) {
            connection->write(command);
        }
    }

    return true;
}

bool CreateNewDocumentCommand::do_command(std::string &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in create new document" << std::endl;

    std::string response = "nu chto tebe skazatt pro sahalin\r\n";
    author->write(response);

    std::shared_ptr<SharedDocumentServer> shared_document(
            new SharedDocumentServer(static_cast<boost::asio::io_context &>(author->getSock().get_executor().context())));
    sharedDocuments_.push_back(shared_document);
    shared_document->startShared();

    return true;
}

CreateNewDocumentCommand::~CreateNewDocumentCommand() {
    std::cerr << "~CreateNewDocumentCommand()" << std::endl;
}