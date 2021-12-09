//
// Created by potry on 11/18/2021.
//

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <cppcoro/task.hpp>

#include "Serializer.h"

template <typename IS, Serializer<IS, std::ostringstream> S>
class Transporter {
protected:
    std::unique_ptr<S> serializer;
    virtual cppcoro::task<> send(std::ostringstream &stream) = 0;

public:
    virtual unsigned int id() = 0;

    template <typename... Args>
    void createSerializer(Args &&...args) {
        serializer = std::make_unique<S>(std::forward<Args>(args)...);
    }

    virtual cppcoro::task<> onReceived(std::string data) = 0;

    template <typename T>
    cppcoro::task<> send(T &data) {
        std::ostringstream output;
        co_await send(serializer->serialize(data, output));
    }
};