//
// Created by tesserakt on 08.12.2021.
//

#include "CommandBus.h"
#include "CommonServer.h"
#include "NetworkCommandExecutor.h"

#define blocking false

int main() {
    jsoncons::polymorphic::Definitions<Command, jsoncons::json>::definitions
            .include<RCommand>();
    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json>::definitions
            .include<NetworkCommand>();
    jsoncons::polymorphic::Definitions<Command, jsoncons::json, std::unique_ptr<Command>>::definitions
            .include<RCommand>();
    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json, std::unique_ptr<RCommand>>::definitions
            .include<NetworkCommand>();

    const auto endpoint = cppcoro::net::ipv4_endpoint::from_string("127.0.0.1:8000").value();
    auto server = std::make_shared<CommonServer>();
    CommandBus bus;
    bus.addExecutor<NetworkCommandExecutor>(server);

    server->createSerializer();
    server->registerCallback([&bus](NetworkCommand &cmd) -> cppcoro::task<> {
        if (cmd.isRollback())
            bus.rollback(cmd);
        else
            bus.execute(cmd);
    });
    cppcoro::sync_wait(cppcoro::when_all(
            server->start(endpoint),
            server->listen()));
}