//
// Created by potry on 11/18/2021.
//

#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Serializer.h"

template <typename IS, Serializer<IS, std::ostringstream> S>
class Transporter {
private:
    std::unique_ptr<S> serializer;

public:
    virtual unsigned int id() = 0;

    template <typename... Args>
    void createSerializer(Args &&...args) {
        serializer = std::make_unique<S>(std::forward<Args>(args)...);
    }

    virtual void onReceived(std::string data) = 0;

    virtual cppcoro::task<> send(std::ostringstream &stream) = 0;

    template <typename T>
    cppcoro::task<> send(T &data) {
        std::ostringstream output;
        co_await send(serializer->serialize(data, output));
    }
};