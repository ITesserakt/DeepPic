//
// Created by tesserakt on 03.11.2021.
//

#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <iostream>

#include "HelloCommand.h"
#include "Serializer.h"

BOOST_CLASS_EXPORT_IMPLEMENT(HelloCommand)
static_assert(Serializable<HelloCommand, boost::archive::text_iarchive>);

void HelloCommand::execute() {
    std::cout << "Hello!" << std::endl;
}
