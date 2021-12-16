#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/log/trivial.hpp>

#include "IManageCommand.h"
#include "CommandConstructor.h"

using json = nlohmann::json;

IManageCommand::IManageCommand(command_t type_command, std::vector<std::shared_ptr<Connection>> *connections) {
    switch (type_command) {
        case GET_DOCUMENT:
            letter_ = new GetDocument(connections);
            break;
        case SHARING_COMMAND:
            letter_ = new SharingCommand(connections);
            break;
        case CREATE_DOCUMENT:
            letter_ = new CreateNewDocumentCommand();
            break;
    }
}

bool IManageCommand::do_command(json &command, std::shared_ptr<Connection> author) {
    letter_->do_command(command, author);

    return true;
}

IManageCommand::~IManageCommand() {
    delete letter_;
}

bool GetDocument::do_command(json &command, std::shared_ptr<Connection> author) {
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

GetDocument::GetDocument(std::vector<std::shared_ptr<Connection>> *connections) {
    connections_ = connections;
}

SharingCommand::SharingCommand(std::vector<std::shared_ptr<Connection>> *connections) {
    connections_ = connections;
}

bool SharingCommand::do_command(json &command, std::shared_ptr<Connection> author) {
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
    BOOST_LOG_TRIVIAL(info) << "Create new document";
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
    BOOST_LOG_TRIVIAL(info) << "~CreateNewDocumentCommand()";
    std::cerr << "~CreateNewDocumentCommand()" << std::endl;
}

DocumentCommandBus::DocumentCommandBus(std::vector<std::shared_ptr<Connection>> *connection) : getDocumentCommand_(GET_DOCUMENT,
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
