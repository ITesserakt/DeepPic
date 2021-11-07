//
// Created by tesserakt on 02.11.2021.
//

#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <sstream>

#include "Serializer.h"

class BoostSerializer {
public:
    using IArchive = boost::archive::text_iarchive;
    using OArchive = boost::archive::text_oarchive;

    template <Serializable<OArchive> T, typename O>
    O &serialize(T &data, O &ostream) {
        OArchive out(ostream);
        out &BOOST_SERIALIZATION_NVP(data);
        return ostream;
    }

    template <Serializable<IArchive> T, typename I>
    T *deserialize(I &&is) {
        IArchive in(is);
        T *result;
        in &BOOST_SERIALIZATION_NVP(result);
        return result;
    }
};

static_assert(Serializer<BoostSerializer, std::istream, std::ostream>);