#include "pch.h"

#include "swarm_algorithm.h"

#include "swarm_numtypes.h"
#include "swarm_particle.h"
#include "swarm_solution.h"
#include "swarm_rndgen.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <execution>
#include <functional>
#include <utility>

using ParticleVector = std::vector<swarm::core::Particle>;
using SolutionVector = std::vector<swarm::core::Solution>;
using std::transform, std::min_element, std::for_each;
using std::abs;
using std::execution::par, std::execution::par_unseq;
using std::move;

namespace swarm::core
{
    static inline void initialize_particles(
        ParticleVector& particles,
        natural_t dim, natural_t particle_count,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t local_weight, real_t global_weight,
        real_t max_speed_mod,
        TargetFunction target_function
    );

    static inline Solution select_global_min(
        ParticleVector& particles
    );

    Solution optimize(
        natural_t dim, natural_t particle_count,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t local_weight, real_t global_weight,
        real_t max_speed_mod,
        size_t iteration_count,
        real_t initial_w, size_t comparation_interval, real_t epsilon,
        TargetFunction target_function
    ) {
        ParticleVector particles { };
        particles.reserve(particle_count);
        initialize_particles(
            particles,
            dim, particle_count,
            min_pos_bound, max_pos_bound,
            local_weight, global_weight,
            max_speed_mod,
            target_function
        );

        Solution current_min { select_global_min(particles) };
        Solution last_min { current_min };
        real_t w { initial_w };
        for (size_t i = 1; i < iteration_count; i++)
        {
            for_each(
                par_unseq,
                particles.begin(), particles.end(),
                [&](Particle& p)
                {
                    p.roam(current_min.pos, w);
                }
            );

            current_min = select_global_min(particles);

            if (i % comparation_interval == 0)
            {
                if (abs(current_min.value - last_min.value) / (real_t)comparation_interval < epsilon)
                    w *= 0.5;
                last_min = current_min;
            }
        } 

        return current_min;
    }

    static inline void initialize_particles(
        ParticleVector& particles,
        natural_t dim, natural_t particle_count,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t local_weight, real_t global_weight,
        real_t max_speed_mod,
        TargetFunction target_function
    ) {
        size_t generator_shift
        {
            FileRandomNumbersGenerator::rget_random_numbers_count() / particle_count
        };

        for (natural_t i = 0; i < particle_count; i++)
        {
            FileRandomNumbersGenerator generator { i * generator_shift };

            Particle p
            {
                dim,
                generator,
                local_weight, global_weight,
                min_pos_bound, max_pos_bound,
                max_speed_mod,
                target_function
            };
            particles.push_back(move(p));
        }
    }

    static inline Solution select_global_min(
        ParticleVector& particles
    ) {
        return (*min_element(
            par_unseq,
            particles.begin(), particles.end(),
            [](const Particle& a, const Particle& b)
            {
                return a.get_local_min().value < b.get_local_min().value;
            }
        )).get_local_min();
    }
}
