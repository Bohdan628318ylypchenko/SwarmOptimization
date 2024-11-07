#pragma once

#include "swarm_numtypes.h"
#include "swarm_solution.h"
#include "swarm_particle.h"

namespace swarm::core
{
    Solution optimize(
        natural_t dim, natural_t particle_count,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t local_weight, real_t global_weight,
        real_t max_speed_mod,
        size_t iteration_count,
        real_t initial_w, size_t comparation_interval,
        TargetFunction target_function
    );
}
