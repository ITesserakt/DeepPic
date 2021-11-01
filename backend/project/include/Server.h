//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <cppcoro/task.hpp>

class Server {
public:
    virtual cppcoro::task<> broadcast() = 0;
};