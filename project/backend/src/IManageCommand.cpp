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
    return true;
}

bool SharingCommand::do_command(std::string &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in sharing command" << std::endl;

    return true;
}

bool CreateNewDocumentCommand::do_command(std::string &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in create new document" << std::endl;

    std::string response = "nu chto tebe skazatt pro sahalin\r\n";
    author->write(response);

    std::shared_ptr <SharedDocumentServer> shared_document(
            new SharedDocumentServer(static_cast<boost::asio::io_context &>(author->getSock().get_executor().context())));
    sharedDocuments_.push_back(shared_document);
    shared_document->startShared();

    return true;
}

CreateNewDocumentCommand::~CreateNewDocumentCommand() {
    std::cerr << "~CreateNewDocumentCommand()" << std::endl;
}