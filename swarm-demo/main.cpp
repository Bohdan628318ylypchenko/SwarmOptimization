#include "pch.h"

#include "swarm_numtypes.h"
#include "swarm_vn.h"
#include "swarm_solution.h"
#include "swarm_rndgen.h"
#include "swarm_algorithm.h"

#include "demo_utility.h"

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>

using namespace swarm::core;

using std::cout, std::endl;
using std::chrono::high_resolution_clock;
using std::pow, std::cos, std::exp, std::sqrt;
using std::vector, std::pair, std::priority_queue;
using std::sort;

namespace swarm::demo
{
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

    inline vector<pair<OptimizationParameters, Solution>> find_optimal_parameters(
        natural_t dim,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t max_speed_mod,
        TargetFunction target_function,
        size_t top_count
    ) {
        const natural_t particle_count_range[]
        {
            //50, 100, 500, 1000, 1500, 2000, 4000
            500, 1000
        };

        const vector<real_t> local_weight_range = range(1, 5, 5);
        const vector<real_t> global_weight_range = range(1, 5, 5);

        const natural_t iteration_count_range[]
        {
            //100, 1000, 2000, 4000, 8000, 10000, 12000, 20000
            1000, 2000
        };

        const real_t initial_w_range[]
        {
            1, 1.2
        };

        const size_t comparation_interval_range[]
        {
            10, 100, 1000
        };

        const real_t epsilon_range[]
        {
            1e-2, 1e-3
        };

        size_t i { 0 };
        auto comparator = [](
            const pair<OptimizationParameters, Solution>& a,
            const pair<OptimizationParameters, Solution>& b
            ) {
                return a.second.value < b.second.value;
            };
        priority_queue<
            pair<OptimizationParameters, Solution>,
            vector<pair<OptimizationParameters, Solution>>,
            decltype(comparator)
        > min_heap(comparator);
        for (auto particle_count : particle_count_range)
        {
            for (auto local_weight : local_weight_range)
            {
                for (auto global_weight : global_weight_range)
                {
                    for (auto iteration_count : iteration_count_range)
                    {
                        for (auto initial_w : initial_w_range)
                        {
                            for (auto comparation_interval : comparation_interval_range)
                            {
                                for (auto epsilon : epsilon_range)
                                {
                                    Solution s
                                    {
                                        optimize(
                                            dim, particle_count,
                                            min_pos_bound, max_pos_bound,
                                            local_weight, global_weight,
                                            max_speed_mod,
                                            iteration_count,
                                            initial_w, comparation_interval, epsilon,
                                            target_function
                                        )
                                    };

                                    OptimizationParameters op { };
                                    op.particle_count = particle_count;
                                    op.local_weight = local_weight;
                                    op.global_weight = global_weight;
                                    op.iteration_count = iteration_count;
                                    op.initial_w = initial_w;
                                    op.comparation_interval = comparation_interval;
                                    op.epsilon = epsilon;

                                    pair<OptimizationParameters, Solution> ops { op, s };
                                    
                                    if (min_heap.size() < top_count)
                                    {
                                        min_heap.push(ops);
                                    }
                                    else if (s.value < min_heap.top().second.value)
                                    {
                                        min_heap.pop();
                                        min_heap.push(ops);
                                    }

                                    if ((++i) % 100 == 0)
                                    {
                                        cout << "processed " << i << " iterations" << endl;
                                        cout <<
                                            "value: " << s.value << "; " <<
                                            "particle_count: " << op.particle_count << "; " <<
                                            "local_weight: " << op.local_weight << "; " <<
                                            "global_weight: " << op.global_weight << "; " <<
                                            "iteration_count: " << op.iteration_count << "; " <<
                                            "initial_w: " << op.initial_w << "; " <<
                                            "comp_interval: " << op.comparation_interval << "; " <<
                                            "e: " << op.epsilon << endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        cout << "total iteration count = " << i << endl;

        vector<pair<OptimizationParameters, Solution>> result { };
        while (!min_heap.empty())
        {
            result.push_back(min_heap.top());
            min_heap.pop();
        }

        return result;
    }

    inline void research(
        natural_t dim,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t max_speed_mod,
        TargetFunction target_function,
        size_t top_count
    ) {
        auto result = swarm::demo::find_optimal_parameters(
            dim, min_pos_bound, max_pos_bound, max_speed_mod, target_function,
            top_count
        );

        sort(
            result.begin(), result.end(),
            [](const pair<swarm::demo::OptimizationParameters, Solution>& a, const pair<swarm::demo::OptimizationParameters, Solution>& b)
            {
                return a.second.value < b.second.value;
            }
        );

        cout << "Top " << top_count << " best parameter sets:" << endl;
        for (size_t i = 0; i < top_count; i++)
        {
            cout << "i = " << i << ": " <<
                "value: " << result[i].second.value << "; " <<
                "particle_count: " << result[i].first.particle_count << "; " <<
                "local_weight: " << result[i].first.local_weight << "; " <<
                "global_weight: " << result[i].first.global_weight << "; " <<
                "iteration_count: " << result[i].first.iteration_count << "; " <<
                "initial_w: " << result[i].first.initial_w << "; " <<
                "comp_interval: " << result[i].first.comparation_interval << "; " <<
                "e: " << result[i].first.epsilon << endl;
        }
    }

    inline void simple_research()
    {
        research(
            50, -100, 100, 20,
            [](const VN& x)
            {
                real_t result { 0 };
                for (natural_t i = 0; i < x.get_dim(); i++)
                    result += pow(x[i], 2.0);
                return result;
            },
            20
        );
    }

    inline void ackley_research()
    {
        research(
            20, -32.768, 32.768, 10,
            [](const VN& x)
            {
                constexpr real_t PI { 3.14159265358979323846 };

                constexpr real_t a { 20 };
                constexpr real_t b { 0.2 };
                constexpr real_t c { 2.0 * PI };
                constexpr real_t d { 20 };

                real_t s1 { 0 }; real_t s2 { 0 };
                for (natural_t i = 0; i < x.get_dim(); i++)
                {
                    s1 += pow(x[i], 2.0);
                    s2 += cos(c * x[i]);
                }
                s1 /= d; s2 /= d;

                return -a * pow(exp(1.0), -b * sqrt(s1)) - pow(exp(1.0), s2) + a + exp(1.0);
            },
            20
        );
    }
}

int main(void)
{
    if (FileRandomNumbersGenerator::initialize_random_numbers_source(
        L"normalized-quantum-big.rnd")
    ) {
        FileRandomNumbersGenerator::free_random_numbers_source();
        return 1;
    }

    cout << "=== simple: ===" << endl;
    swarm::demo::simple_research();
    cout << "=== ackley: ===" << endl;
    swarm::demo::ackley_research();

    FileRandomNumbersGenerator::free_random_numbers_source();
    return 0;
}
