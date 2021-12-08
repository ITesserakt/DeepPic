#include "CommandConstructor.h"

json CommandConstructor::sharingDocumentClient() {
    static json command = {{"target", "sharing_document"}};
    return command;
}

json CommandConstructor::sharingDocumentServer(std::string &&status, std::string &&auth_token, std::string &&address, int port) {
    json command = {{"status",     status},
                    {"target",     "sharing_document"},
                    {"auth_token", auth_token},
                    {"address",    address},
                    {"port",       port}};

    return command;
}

json CommandConstructor::authClient(std::string &&auth_token) {
    json command = {{"target",     "auth"},
                    {"auth_token", auth_token}};

    return command;
}

json CommandConstructor::authServer(std::string &&status) {
    json command = {{"status", status},
                    {"target", "auth"}};

    return command;
}

json CommandConstructor::getDocumentClient() {
    static json command = {{"target", "get_document"}};

    return command;
}

json CommandConstructor::getDocumentServer(std::string &&status, std::string &&document) {
    json command = {{"status",   status},
                    {"target",   "get_document"},
                    {"document", document}};

    return command;
}

json CommandConstructor::sharingCommandClient(std::string &&command_) {
    json command = {{"target",  "sharing_command"},
                    {"command", command_}};

    return command;
}
