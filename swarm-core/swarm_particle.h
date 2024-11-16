#pragma once

#include "swarm_numtypes.h"
#include "swarm_vn.h"
#include "swarm_rndgen.h"
#include "swarm_solution.h"

#include <functional>

namespace swarm::core
{
    using TargetFunction = std::function<real_t(const VN&)>;

    class Particle
    {
    private:
        static constexpr real_t MIN_SPEED { 1e-9 };

        natural_t dim;

        VN pos;
        VN speed;
        FileRandomNumbersGenerator generator;

        VN r1; VN r2;
        VN local_min_copy; VN global_min_copy;

        Solution local_min;

        const real_t local_weight;
        const real_t global_weight;

        const real_t min_pos_bound;
        const real_t max_pos_bound;
        const real_t max_speed_mod;

        const TargetFunction target_function;

        void adjust_if_out_of_bounds(VN& vn);

    public:
        Particle();
        Particle(
            natural_t dim,
            FileRandomNumbersGenerator generator,
            real_t local_weight, real_t global_weight,
            real_t min_pos_bound, real_t max_pos_bound,
            real_t max_speed_mod,
            const TargetFunction target_function
        );
        
        Solution get_local_min() const noexcept;

        void roam(const VN& global_min_pos, real_t w);
    };
}
