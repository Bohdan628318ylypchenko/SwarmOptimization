#pragma once

#include "swarm_numtypes.h"

namespace swarm::core
{
    class VN
    {
    private:
        static constexpr real_t EPSILON { 1e-4 };
        real_t* coordinates;
        natural_t dim;
    public:
        VN();
        VN(natural_t dim);
        VN(natural_t dim, real_t* coordinates);

        VN(const VN& other);
        VN(VN&& other) noexcept;

        ~VN();

        real_t* rget_data() const noexcept;
        natural_t get_dim() const noexcept;

        VN& operator=(const VN& other);
        VN& operator=(VN&& other) noexcept;

        real_t& operator[](natural_t i);
        const real_t& operator[](natural_t i) const;

        bool operator==(const VN& other) const;

        real_t mod() const noexcept;

        VN& mut_add(const VN& other);
        VN& mut_subtract(const VN& other);
        VN& mut_multiply_on_scalar(real_t k);
        VN& mut_coordinate_multiply(const VN& other);
        VN& mut_normalize();

        VN copy() const;
    };
}
