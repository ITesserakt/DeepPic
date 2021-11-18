//
// Created by tesserakt on 30.10.2021.
//

#include "CommandBus.h"
#include "HelloCommand.h"
#include "LocalCommandExecutor.h"
#include "NetworkCommandExecutor.h"

int main(int argc, char **argv) {
    auto bus = CommandBus();
}