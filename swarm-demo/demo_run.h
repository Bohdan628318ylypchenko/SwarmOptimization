#pragma once

#include "target_functions.h"
#include "swarm_solution.h"
#include "swarm_numtypes.h"

#include <iostream>

namespace swarm::demo
{
    inline void demo_run_full(OptimizationParameters op, DemoTargetFunction dtf)
    {
        swarm::core::Solution result { dtf(op) };

        std::cout << "value = " << result.value << std::endl;
        for (swarm::natural_t i = 0; i < result.pos.get_dim(); i++)
        {
            std::cout << "coord " << i << " = " << result.pos[i] << std::endl;
        }
    }

    inline void demo_run_value(OptimizationParameters op, DemoTargetFunction dtf)
    {
        swarm::core::Solution result { dtf(op) };

        std::cout << "value = " << result.value << std::endl;
    }
}
