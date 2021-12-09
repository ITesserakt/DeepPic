//
// Created by tesserakt on 09.12.2021.
//

#pragma once

#include "Client.h"
#include "JsonSerializer.h"
#include "command/NetworkCommand.h"

class CommonClient : public Client<JsonSerializer> {
private:
    std::vector<std::function<cppcoro::task<>(NetworkCommand &)>> callbacks_;

public:
    cppcoro::task<> send(std::ostringstream &data) override {
        auto str = data.str();
        std::size_t written = 0;
        do {
            written += co_await socket->send(str.data() + written, str.size() - written, source.token());
            std::cout << "Sending next " << written << " bytes" << std::endl;
        } while (written < str.size());
    }

    template <typename F>
    void registerCallback(F &&fn) {
        callbacks_.emplace_back(std::forward<F>(fn));
    }

    cppcoro::task<> onReceived(std::string data) override {
        std::cout << "Received: " << data << std::endl;
        NetworkCommand cmd = serializer->deserialize<NetworkCommand>(data);
        for (const auto &callback : callbacks_)
            co_await callback(cmd);
    }

    void shutdown() {
        source.request_cancellation();
        async.stop();
    }
};