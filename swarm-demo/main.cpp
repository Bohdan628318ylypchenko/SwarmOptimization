#include "pch.h"

#include "swarm_numtypes.h"
#include "swarm_vn.h"
#include "swarm_solution.h"
#include "swarm_rndgen.h"
#include "swarm_algorithm.h"

#include <iostream>
#include <chrono>
#include <cmath>

using namespace swarm::core;

using std::cout, std::endl;
using std::chrono::high_resolution_clock;

using std::pow, std::cos, std::exp, std::sqrt;

namespace swarm::demo
{
    inline void demo(
        natural_t dim, natural_t particle_count,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t local_weight, real_t global_weight,
        real_t max_speed_mod,
        size_t iteration_count,
        real_t initial_w, size_t comparation_interval,
        TargetFunction target_function
    ) {
        auto start { high_resolution_clock::now() };
        Solution result
        {
            optimize(
                dim, particle_count,
                min_pos_bound, max_pos_bound,
                local_weight, global_weight,
                max_speed_mod,
                iteration_count,
                initial_w, comparation_interval,
                target_function
            )
        };
        auto end { high_resolution_clock::now() };

        for (natural_t i = 0; i < result.pos.get_dim(); i++)
        {
            cout << "coord " << i << " = " << result.pos[i] << endl;
        }
        cout << "Value = " << result.value << endl;

        auto duration { std::chrono::duration_cast<std::chrono::milliseconds>(end - start) };
        cout << "Time taken: " << duration.count() << " ms" << endl;
    }

    inline void simple()
    {
        cout << "=== simple ===" << endl;
        demo(
            50, 1500,
            -100, 100,
            1, 4,
            20,
            10000,
            1, 100,
            [](const VN& x)
            {
                real_t result { 0 };
                for (natural_t i = 0; i < x.get_dim(); i++)
                    result += pow(x[i], 2.0);
                return result;
            }
        );
    }

    inline void ackley()
    {
        cout << "=== ackley ===" << endl;
        demo(
            20, 1500,
            -32.768, 32.768,
            1, 4,
            20,
            10000,
            1, 100,
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
            }
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

    //swarm::demo::simple();
    //cout << "\n";
    swarm::demo::ackley();

    FileRandomNumbersGenerator::free_random_numbers_source();
    return 0;
}
