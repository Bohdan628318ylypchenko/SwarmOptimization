#include "pch.h"

#include "swarm_particle.h"

#include "swarm_numtypes.h"
#include "swarm_vn.h"
#include "swarm_rndgen.h"
#include "swarm_solution.h"

#include <functional>
#include <utility>

using std::move;

namespace swarm::core
{
    Particle::Particle():
        dim { 0 },
        pos { }, speed { },
        generator { 0 },
        local_min { },
        local_weight { }, global_weight { },
        min_pos_bound { },
        max_pos_bound { },
        max_speed_mod { },
        target_function { }
    { }

    Particle::Particle(
        natural_t dim,
        FileRandomNumbersGenerator generator,
        real_t local_weight, real_t global_weight,
        real_t min_pos_bound, real_t max_pos_bound,
        real_t max_speed_mod,
        const TargetFunction target_function
    ):
        dim { dim },
        pos { dim }, speed { dim },
        generator { generator },
        local_min { },
        local_weight { local_weight },
        global_weight { global_weight },
        min_pos_bound { min_pos_bound },
        max_pos_bound { max_pos_bound },
        max_speed_mod { max_speed_mod },
        target_function { target_function }
    {
        this->generator.next_n_uniform_double(
            pos.get_dim(), pos.rget_data(),
            this->min_pos_bound,
            this->max_pos_bound
        );

        this->generator.next_n_uniform_double(
            speed.get_dim(), speed.rget_data(),
            -1, 1
        );

        speed.mut_multiply_on_scalar(
            generator.next_uniform_double(MIN_SPEED, this->max_speed_mod)
        );

        VN next_pos = pos.copy().mut_add(speed);

        real_t current_pos_val { target_function(pos) };
        real_t next_pos_val { target_function(next_pos) };

        if (current_pos_val < next_pos_val)
        {
            local_min.pos = pos;
            local_min.value = current_pos_val;
        }
        else
        {
            local_min.pos = next_pos;
            local_min.value = next_pos_val;
        }

        pos = move(next_pos);
    }

    void Particle::adjust_if_out_of_bounds(VN& vn)
    {
        for (natural_t i = 0; i < pos.get_dim(); i++)
        {
            if (vn[i] < min_pos_bound)
            {
                vn[i] = min_pos_bound;
                continue;
            }

            if (vn[i] > max_pos_bound)
            {
                vn[i] = max_pos_bound;
            }
        }
    }

    Solution Particle::get_local_min() const noexcept
    {
        return local_min;
    }

    void Particle::roam(const VN& global_min_pos, real_t w)
    {
        VN r1 { dim };
        generator.next_n_uniform_double(
            r1.get_dim(), r1.rget_data()
        );

        VN r2 { dim };
        generator.next_n_uniform_double(
            r2.get_dim(), r2.rget_data()
        );

        speed
            .mut_multiply_on_scalar(w)
            .mut_add(
                r1.mut_multiply_on_scalar(local_weight)
                  .mut_coordinate_multiply(local_min.pos.copy().mut_subtract(pos))
            )
            .mut_add(
                r2.mut_multiply_on_scalar(global_weight)
                  .mut_coordinate_multiply(global_min_pos.copy().mut_subtract(pos))
            );

        if (speed.mod() >= max_speed_mod)
        {
            speed.mut_normalize()
                 .mut_multiply_on_scalar(max_speed_mod);
        }

        adjust_if_out_of_bounds(pos.mut_add(speed));

        real_t pos_value { target_function(pos) };
        if (pos_value < local_min.value)
        {
            local_min.pos = pos;
            local_min.value = pos_value;
        }
    }
}