#include "pch.h"

#include "target_functions.h"

#include "swarm_numtypes.h"
#include "swarm_vn.h"
#include "swarm_solution.h"
#include "swarm_algorithm.h"

#include <cmath>

using std::pow, std::cos, std::sqrt;
using swarm::natural_t, swarm::real_t;
using swarm::core::VN;
using swarm::core::Solution;
using swarm::core::optimize;

namespace swarm::demo
{
    Solution simple(const OptimizationParameters& op)
    {
        return optimize(
            50, op.particle_count,
            -100, 100,
            op.local_weight, op.global_weight,
            20,
            op.iteration_count,
            op.initial_w, op.comparation_interval, op.epsilon,
            [](const VN& x)
            {
                real_t result { 0 };
                for (natural_t i = 0; i < x.get_dim(); i++)
                    result += pow(x[i], 2.0);
                return result;
            }
        );
    }

    Solution ackley(const OptimizationParameters& op)
    {
        return optimize(
            20, op.particle_count,
            -32.768, 32.768,
            op.local_weight, op.global_weight,
            10,
            op.iteration_count,
            op.initial_w, op.comparation_interval, op.epsilon,
            [](const VN& x)
            {
                const real_t a { 20 };
                const real_t b { 0.2 };
                const real_t c { 2.0 * PI };
                const real_t d { 20 };

                real_t s1 { 0 }; real_t s2 { 0 };
                for (natural_t i = 0; i < x.get_dim(); i++)
                {
                    s1 += pow(x[i], 2.0);
                    s2 += cos(c * x[i]);
                }
                s1 /= d; s2 /= d;

                return -a * pow(E, -b * sqrt(s1)) - pow(E, s2) + a + E;
            }
        );
    }

    Solution griewank(const OptimizationParameters& op)
    {
        return optimize(
            50, op.particle_count,
            -600, 600,
            op.local_weight, op.global_weight,
            300,
            op.iteration_count,
            op.initial_w, op.comparation_interval, op.epsilon,
            [](const VN& x)
            {
                real_t t1 { 0 };

                for (natural_t i = 0; i < x.get_dim(); i++)
                {
                    t1 += (pow(x[i], 2.0) / 4000);
                }

                real_t t2 { 1 };
                for (natural_t i = 0; i < x.get_dim(); i++)
                {
                    t2 *= cos(x[i] / sqrt(i + 1));
                }

                return t1 - t2 + 1;
            }
        );
    }

    Solution rastrigin(const OptimizationParameters& op)
    {
        return optimize(
            30, op.particle_count,
            -5.12, 5.12,
            op.local_weight, op.global_weight,
            2.5,
            op.iteration_count,
            op.initial_w, op.comparation_interval, op.epsilon,
            [](const VN& x)
            {
                real_t t { 0 };

                for (natural_t i = 0; i < x.get_dim(); i++)
                {
                    t += (pow(x[i], 2) - 10.0 * cos(2.0 * PI * x[i]));
                }

                return 10.0 * x.get_dim() + t;
            }
        );
    }

    Solution rosenbrock(const OptimizationParameters& op)
    {
        return optimize(
            30, op.particle_count,
            -5, 10,
            op.local_weight, op.global_weight,
            4,
            op.iteration_count,
            op.initial_w, op.comparation_interval, op.epsilon,
            [](const VN& x)
            {
                real_t t { 0 };

                for (natural_t i = 0; i < x.get_dim() - 1; i++)
                {
                    t += (pow(100 * (x[i + 1] - pow(x[i], 2)), 2) + pow(x[i] - 1, 2));
                }

                return t;
            }
        );
    }
}