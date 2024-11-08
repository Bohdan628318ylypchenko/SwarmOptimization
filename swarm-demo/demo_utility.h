#pragma once

#include "swarm_numtypes.h"

#include <vector>
#include <numeric>
#include <algorithm>

namespace swarm::demo
{
    inline std::vector<real_t> range(real_t start, real_t end, natural_t count)
    {
        std::vector<real_t> result(count);

        real_t step { (end - start) / ((real_t)count - 1) };
        std::iota(result.begin(), result.end(), 0);
        std::transform(
            result.begin(), result.end(), result.begin(),
            [start, step](real_t i)
            {
                return start + i * step;
            }
        );

        return result;
    }
}
