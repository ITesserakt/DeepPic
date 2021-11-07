//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <cppcoro/task.hpp>
#include <memory>

#include "BoostSerializer.h"
#include "Serializer.h"

using IStream = std::istringstream;
using OStream = std::ostringstream;

template <Serializer<IStream, OStream> S>
class Server {
private:
    std::unique_ptr<S> serializer;

protected:
    virtual cppcoro::task<> broadcast(std::string) = 0;

public:
    virtual unsigned int id() = 0;

    template <typename T>
    cppcoro::task<> broadcast(T value) {
        std::ostringstream os;
        co_await broadcast(serializer->serialize(value, os).str());
    }

    template <typename... Args>
    void createSerializer(Args &&...args) {
        serializer = std::make_unique<S>(std::forward<Args>(args)...);
    }
};
