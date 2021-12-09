//
// Created by tesserakt on 30.10.2021.
//

#include <QApplication>

#include "CommandBus.h"
#include "CommonClient.h"
#include "MainWindow.h"
#include "NetworkCommandExecutor.h"
#include "UICommandExecutor.h"

#define blocking true

int main(int argc, char **argv) {
    jsoncons::polymorphic::Definitions<Command, jsoncons::json>::definitions
            .include<RCommand>()
            .include<command::BrushStarted>()
            .include<command::BrushMoved>();
    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json>::definitions
            .include<NetworkCommand>();
    jsoncons::polymorphic::Definitions<Command, jsoncons::json, std::unique_ptr<Command>>::definitions
            .include<RCommand>()
            .include<command::BrushStarted>()
            .include<command::BrushMoved>();
    jsoncons::polymorphic::Definitions<RCommand, jsoncons::json, std::unique_ptr<RCommand>>::definitions
            .include<NetworkCommand>();

    const auto endpoint = cppcoro::net::ip_endpoint::from_string("127.0.0.1:8000").value();
    QApplication a(argc, argv);

    auto client = std::make_shared<CommonClient>();
    CommandBus globalCommandBus;
    PaintScene scene{globalCommandBus};
    MainWindow window{scene};

    client->registerCallback([&globalCommandBus](NetworkCommand &cmd) -> cppcoro::task<> {
        if (cmd.isRollback())
            globalCommandBus.rollback(cmd);
        else
            globalCommandBus.execute(cmd);
    });

    globalCommandBus.addExecutor<UICommandExecutor>(scene);
    globalCommandBus.addExecutor<NetworkCommandExecutor>(client);

    window.setMinimumSize(500, 500);
    window.setWindowTitle("DeepPic");
    window.showMaximized();

    cppcoro::sync_wait(cppcoro::when_all(
            client->connectTo(endpoint),
            [&]() -> cppcoro::task<> {
                a.exec();
                client->shutdown();
                co_return;
            }()));
}