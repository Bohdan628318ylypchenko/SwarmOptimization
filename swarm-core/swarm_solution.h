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

        Solution(VN pos, real_t value):
            pos { pos }, value { value }
        { }

        Solution(const Solution& other):
            pos { other.pos }, value { other.value }
        { }
    };
}
