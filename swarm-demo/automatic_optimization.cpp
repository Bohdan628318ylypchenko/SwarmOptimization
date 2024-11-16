#include "pch.h"

#include "automatic_optimization.h"

#include "target_functions.h"
#include "swarm_algorithm.h"
#include "swarm_numtypes.h"
#include "swarm_solution.h"

#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

using swarm::core::Solution;

using std::cout, std::endl;
using std::vector, std::pair, std::priority_queue;
using std::sort, std::iota, std::transform;

namespace swarm::demo
{
    static inline void research(DemoTargetFunction target_function, size_t top_count);
    static inline vector<real_t> range(real_t start, real_t end, natural_t count);

    void simple_research()
    {
        research(simple, 20);
    }

    void ackley_research()
    {
        research(ackley, 20);
    }

    void griewank_research()
    {
        research(griewank, 20);
    }

    void rastrigin_research()
    {
        research(rastrigin, 20);
    }

    void rosenbrock_research()
    {
        research(rosenbrock, 20);
    }

    static inline void research(DemoTargetFunction target_function, size_t top_count)
    {
        const natural_t particle_count_range[]
        {
            100, 1000, 2000, 4000
            //500, 1000
        };

        const real_t weight_sum = 6;
        const vector<real_t> local_weight_range = range(1, 5, 5);

        const natural_t iteration_count_range[]
        {
            100, 1000, 5000, 10000, 20000
            //2000
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
        OptimizationParameters op;
        for (auto particle_count : particle_count_range)
        {
            op.particle_count = particle_count;
            for (auto local_weight : local_weight_range)
            {
                op.local_weight = local_weight;
                op.global_weight = weight_sum - local_weight;
                for (auto iteration_count : iteration_count_range)
                {
                    op.iteration_count = iteration_count;
                    for (auto initial_w : initial_w_range)
                    {
                        op.initial_w = initial_w;
                        for (auto comparation_interval : comparation_interval_range)
                        {
                            op.comparation_interval = comparation_interval;
                            for (auto epsilon : epsilon_range)
                            {
                                op.epsilon = epsilon;

                                Solution s { target_function(op) };

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

        cout << "total iteration count = " << i << endl;

        vector<pair<OptimizationParameters, Solution>> result { };
        while (!min_heap.empty())
        {
            result.push_back(min_heap.top());
            min_heap.pop();
        }

        sort(
            result.begin(), result.end(),
            [](const pair<OptimizationParameters, Solution>& a, const pair<OptimizationParameters, Solution>& b)
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

    static inline vector<real_t> range(real_t start, real_t end, natural_t count)
    {
        vector<real_t> result(count);

        real_t step { (end - start) / ((real_t)count - 1) };
        iota(result.begin(), result.end(), 0);
        transform(
            result.begin(), result.end(), result.begin(),
            [start, step](real_t i)
            {
                return start + i * step;
            }
        );

        return result;
    }
}