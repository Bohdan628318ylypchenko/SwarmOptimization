#pragma once

#include "swarm_numtypes.h"

#include <memory>

namespace swarm::core
{
    class VN
    {
    private:
        static constexpr real_t EPSILON { 1e-4 };
        std::unique_ptr<real_t[]> coordinates;
    public:
        const natural_t dim;

        VN(natural_t dim);
        VN(natural_t dim, real_t* coordinates);

        VN(const VN& other);
        VN(VN&& other) noexcept;

        real_t* rget_data() const noexcept;

        real_t& operator[](natural_t i);
        const real_t& operator[](natural_t i) const;

        bool operator==(const VN& other) const;

        VN& operator+=(const VN& other);
        VN operator+(const VN& other) const;

        VN& operator-=(const VN& other);
        VN operator-(const VN& other) const;

        VN& operator*=(real_t k);
        VN operator*(real_t k) const;

        real_t mod() const noexcept;

        VN& mut_normalize();
        VN& mut_coordinate_multiply(const VN& other);
    };
}
