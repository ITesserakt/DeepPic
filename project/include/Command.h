#pragma once

#include <nlohmann/json.hpp>

#include "Settings.h"
#include "SharedDocumentServer.h"
#include "Connection.h"

using json = nlohmann::json;

typedef enum {
    SHARING_COMMAND = 0x01,
    GET_DOCUMENT = 0x02,
    CREATE_DOCUMENT = 0x04
} command_t;

class Command {
public:
    Command(command_t type_command, std::vector<std::shared_ptr<Connection>> *connection = nullptr);

    Command(const Command &) = delete;

    Command &operator=(Command &) = delete;

    virtual bool do_command(json &command, std::shared_ptr<Connection> author);

    ~Command();

protected:
    Command() : letter_(nullptr) {}

private:
    Command *letter_;
};

class GetDocument : public Command {
public:
    virtual bool do_command(json &command, std::shared_ptr<Connection> author) override;

    GetDocument(const GetDocument &) = delete;

    GetDocument &operator=(GetDocument &) = delete;

    ~GetDocument() = default;

private:
    friend class Command;

    explicit GetDocument(std::vector<std::shared_ptr<Connection>> *connections);

    void getDocumentFromClient();

    void handleGetDocumentFromClient(std::shared_ptr<Connection> &connection);

    void sendDocumentToNewClients(std::string &&document);

    std::vector<std::shared_ptr<Connection>> clientsToGetDocument_;

    std::vector<std::shared_ptr<Connection>> *connections_ = nullptr;
};

class SharingCommand : public Command {
public:
    virtual bool do_command(json &command, std::shared_ptr<Connection> author) override;

    SharingCommand(const SharingCommand &) = delete;

    SharingCommand &operator=(SharingCommand &) = delete;

    ~SharingCommand() = default;

private:
    friend class Command;

    explicit SharingCommand(std::vector<std::shared_ptr<Connection>> *connections);

    std::vector<std::shared_ptr<Connection>> *connections_ = nullptr;
};

class CreateNewDocumentCommand : public Command {
public:
    virtual bool do_command(json &command, std::shared_ptr<Connection> author) override;

    ~CreateNewDocumentCommand();

private:
    friend class Command;

    CreateNewDocumentCommand() = default;

    std::vector<std::shared_ptr<SharedDocumentServer>> sharedDocuments_;
};

class DocumentCommandBus {
public:
    explicit DocumentCommandBus(std::vector<std::shared_ptr<Connection>> *connection);
    bool do_command(std::string &&command, std::shared_ptr<Connection> author);
private:
    Command sharingCommand_;
    Command getDocumentCommand_;
};