#include <iostream>
#include <nlohmann/json.hpp>

#include "IManageCommand.h"
#include "CommandConstructor.h"

using json = nlohmann::json;

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

bool IManageCommand::do_command(json &command, std::shared_ptr<Connection> author) {
    letter_->do_command(command, author);

    return true;
}

IManageCommand::~IManageCommand() {
    delete letter_;
}

bool GetDocument::do_command(json &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in get document" << std::endl;
    if (command["target"] == "get_document" && !command.contains("status")) {
        if (command.contains("status") && command["status"] == "OK") {
            sendDocumentToNewClients(std::move(command["document"]));
            return true;
        } else if (!command.contains("status")) {
            clientsToGetDocument_.push_back(author);
            getDocumentFromClient();
            return true;
        }
    }

    return false;
}

void GetDocument::getDocumentFromClient() {
    std::string command_dump = CommandConstructor::getDocumentClient().dump();
    (*(connections_->begin()))->write(command_dump,
                                      [this](std::shared_ptr<Connection> connection) { this->handleGetDocumentFromClient(connection); });
}

void GetDocument::handleGetDocumentFromClient(std::shared_ptr<Connection> &connection) {
    if (connection == nullptr) {
        getDocumentFromClient();
    }
}

void GetDocument::sendDocumentToNewClients(std::string &&document) {
    std::string command_dump = CommandConstructor::getDocumentServer("OK", std::move(document)).dump();
    for (auto &client: clientsToGetDocument_) {
        client->write(command_dump);
    }
}

bool SharingCommand::do_command(json &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in sharing command" << std::endl;
    for (auto &connection: *connections_) {
        if (author != connection) {
            std::cerr << "SharingCommand::do_command()" << std::endl;
            std::string command_dump = command.dump();
            connection->write(command_dump);
        }
    }

    return true;
}

bool CreateNewDocumentCommand::do_command(json &command, std::shared_ptr<Connection> author) {
    std::cerr << "we are in create new document" << std::endl;
    if (SharedDocumentServer::start_since_port > 6070) {
        return false;
    }

    std::shared_ptr<SharedDocumentServer> shared_document(
            new SharedDocumentServer(static_cast<boost::asio::io_context &>(author->getSock().get_executor().context())));
    sharedDocuments_.push_back(shared_document);
    shared_document->startShared();

    std::string command_dump = CommandConstructor::sharingDocumentServer("OK", shared_document->getAuthToken(), "127.0.0.1",
                                                                         shared_document->getPort()).dump();
    author->write(command_dump);

    return true;
}

CreateNewDocumentCommand::~CreateNewDocumentCommand() {
    std::cerr << "~CreateNewDocumentCommand()" << std::endl;
}

DocumentCommandBus::DocumentCommandBus(std::vector<std::shared_ptr<Connection>> *connection) : getDocumentCommand_(CREATE_DOCUMENT,
                                                                                                                   connection),
                                                                                               sharingCommand_(SHARING_COMMAND,
                                                                                                               connection) {

}

bool DocumentCommandBus::do_command(std::string &&command, std::shared_ptr<Connection> author) {
    try {
        auto command_json = json::parse(command);
        if (command_json.contains("target")) {
            if (command_json["target"] == "sharing_command" && command_json.contains("command")) {
                return sharingCommand_.do_command(command_json, author);
            } else if (command_json["target"] == "get_document") {
                return getDocumentCommand_.do_command(command_json, author);
            }
        }
        return false;
    } catch (...) {
        return false;
    }

}
