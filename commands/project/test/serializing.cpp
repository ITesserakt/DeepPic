//
// Created by potry on 11/18/2021.
//

#include <gtest/gtest.h>
#include <memory>

#include "BoostSerializer.h"
#include "command/Command.h"

using SerializerType = BoostSerializer;

class HelloCommand : public Command {
private:
    friend class boost::serialization::access;

    void serialize(auto &ar, const unsigned int) {
        ar &boost::serialization::base_object<Command>(*this) & dummy;
    }

    int dummy;

public:
    void execute() override;
    explicit HelloCommand(int dummy) : dummy(dummy) {}

    bool operator==(const HelloCommand &rhs) const {
        return dummy == rhs.dummy;
    }
    bool operator!=(const HelloCommand &Rhs) const {
        return !(Rhs == *this);
    }
};

namespace boost::serialization {
    void load_construct_data(auto &ar, HelloCommand *p, const unsigned int) {
        int d;
        ar &d;
        ::new (p) HelloCommand(d);
    }
}// namespace boost::serialization

BOOST_CLASS_EXPORT(HelloCommand)
static_assert(Serializable<HelloCommand, boost::archive::text_iarchive>);

void HelloCommand::execute() {
    std::cout << "Hello!" << std::endl;
}

TEST(serializing, serialize_command) {
    auto serializer = SerializerType();
    auto stream = std::stringstream();
    auto value = HelloCommand(5);

    EXPECT_NO_THROW(serializer.serialize(value, stream));
    EXPECT_EQ(*serializer.deserialize<HelloCommand *>(stream), value);
}