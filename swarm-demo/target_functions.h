#pragma once

#include "swarm_numtypes.h"
#include "swarm_vn.h"
#include "swarm_solution.h"

#include <cmath>
#include <functional>

namespace swarm::demo
{
    const real_t PI { 3.14159265358979323846 };

    const real_t E { std::exp(1.0) };

    struct OptimizationParameters
    {
        natural_t particle_count;
        real_t local_weight;
        real_t global_weight;
        size_t iteration_count;
        real_t initial_w;
        size_t comparation_interval;
        real_t epsilon;
    };

    using DemoTargetFunction = std::function<swarm::core::Solution(const OptimizationParameters&)>;

    swarm::core::Solution simple(const OptimizationParameters& op);

    swarm::core::Solution ackley(const OptimizationParameters& op);

    swarm::core::Solution griewank(const OptimizationParameters& op);

    swarm::core::Solution rastrigin(const OptimizationParameters& op);

    swarm::core::Solution rosenbrock(const OptimizationParameters& op);
}
