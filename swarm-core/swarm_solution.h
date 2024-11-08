#pragma once

#include "swarm_numtypes.h"
#include "swarm_vn.h"

#include <limits>

namespace swarm::core
{
    struct Solution
    {
        VN pos;
        real_t value;

        Solution():
            pos { }, value { std::numeric_limits<real_t>::lowest() }
        { }
    };
}
