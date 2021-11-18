#pragma once

#include "Settings.h"
#include "SharedDocument.h"

enum {
    SHARING_COMMAND = 0x01,
    GET_DOCUMENT = 0x02
} command_t;

class IManageCommand {
public:
    IManageCommand(command_t type_command, SharedDocument *document);

    IManageCommand(const IManageCommand &) = delete;

    IManageCommand &operator=(IManageCommand &) = delete;

    virtual bool do_command(std::string &command, DocumentClient *author);

    ~IManageCommand();

protected:
    IManageCommand() : letter_(NULL);


    SharedDocument *document_;

private:
    IManageCommand *letter_;
};

class GetDocument : public IManageCommand {
public:
    virtual bool do_command(std::string &command, DocumentClient *author);

    GetDocument(const GetDocument &) = delete;

    GetDocument &operator=(GetDocument &) = delete;

    ~GetDocument();

private:
    friend class IManageCommand;

    GetDocument();

    void getDocumentFromClient();

    void handleGetDocumentFromClient(const boost::system::error_code &err, std::size_t bytes_transferred);

    void sendDocumentToNewClients();

    char[MAX_DOCUMENT_LENGTH] document_;

    std::vector<std::shared_ptr<DocumentClient>> clientsToGetDocument_;
};

class SharingCommand : public IManageCommand {
public:
    virtual bool do_command(std::string &command, DocumentClient *author);

    SharingCommand(const SharingCommand &) = delete;

    SharingCommand &operator=(SharingCommand &) = delete;

    ~SharingCommand();

private:
    friend class IManageCommand;

    SharingCommand();
};