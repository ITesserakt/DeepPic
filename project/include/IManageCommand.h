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

class IManageCommand {
public:
    IManageCommand(command_t type_command);

    IManageCommand(command_t type_command, std::vector<std::shared_ptr<Connection>> *connection);

    IManageCommand(const IManageCommand &) = delete;

    IManageCommand &operator=(IManageCommand &) = delete;

    virtual bool do_command(json &command, std::shared_ptr<Connection> author);

    ~IManageCommand();

protected:
    IManageCommand() : letter_(NULL) {}

    std::vector<std::shared_ptr<Connection>> *connections_ = nullptr;

private:
    IManageCommand *letter_;
};

class GetDocument : public IManageCommand {
public:
    virtual bool do_command(json &command, std::shared_ptr<Connection> author) override;

    GetDocument(const GetDocument &) = delete;

    GetDocument &operator=(GetDocument &) = delete;

    ~GetDocument() = default;

private:
    friend class IManageCommand;

    GetDocument() = default;

    void getDocumentFromClient();

    void handleGetDocumentFromClient(std::shared_ptr<Connection> &connection);

    void sendDocumentToNewClients(std::string &&document);

    std::vector<std::shared_ptr<Connection>> clientsToGetDocument_;
};

class SharingCommand : public IManageCommand {
public:
    virtual bool do_command(json &command, std::shared_ptr<Connection> author) override;

    SharingCommand(const SharingCommand &) = delete;

    SharingCommand &operator=(SharingCommand &) = delete;

    ~SharingCommand() = default;

private:
    friend class IManageCommand;

    SharingCommand() = default;
};

class CreateNewDocumentCommand : public IManageCommand {
public:
    virtual bool do_command(json &command, std::shared_ptr<Connection> author) override;

    ~CreateNewDocumentCommand();

private:
    friend class IManageCommand;

    CreateNewDocumentCommand() = default;

    std::vector<std::shared_ptr<SharedDocumentServer>> sharedDocuments_;
};

class DocumentCommandBus {
public:
    explicit DocumentCommandBus(std::vector<std::shared_ptr<Connection>> *connection);
    bool do_command(std::string &&command, std::shared_ptr<Connection> author);
private:
    IManageCommand sharingCommand_;
    IManageCommand getDocumentCommand_;
};