//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <any>
#include <cppcoro/task.hpp>

class Server {
public:
    virtual unsigned int id() = 0;

    virtual cppcoro::task<> broadcast(std::any) = 0;

    template <typename T>
    cppcoro::task<> broadcast(T value) {
        co_await broadcast({value});
    }
};