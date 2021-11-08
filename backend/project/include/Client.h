//
// Created by tesserakt on 01.11.2021.
//

#pragma once

#include <memory>

#include "Serializer.h"

using IStream = std::istringstream;
using OStream = std::ostringstream;

template <Serializer<IStream, OStream> S>
class Client {
private:
    std::unique_ptr<S> serializer;

public:
    template <typename... Args>
    void createSerializer(Args &&...args) {
        serializer = std::make_unique<S>(std::forward<Args>(args)...);
    }

    virtual void receive(std::string data) = 0;
};