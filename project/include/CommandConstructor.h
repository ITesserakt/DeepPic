#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CommandConstructor {
public:
    static json sharingDocumentClient();

    static json sharingDocumentServer(std::string &&status, std::string &&auth_token, std::string &&address, int port);

    static json authClient(std::string &&auth_token);

    static json authServer(std::string &&status);

    static json getDocumentClient();

    static json getDocumentServer(std::string &&status, std::string &&document);

    static json sharingCommandClient(std::string &&command_);
};