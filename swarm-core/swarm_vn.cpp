#include "pch.h"

#include "swarm_vn.h"

#include "swarm_numtypes.h"

#include <memory>
#include <algorithm>
#include <cmath>

using std::unique_ptr;
using std::copy_n;
using std::pow, std::sqrt, std::abs;

namespace swarm::core
{
    VN::VN(natural_t dim):
        dim { dim },
        coordinates { new real_t[dim] }
    { }

    VN::VN(natural_t dim, real_t* coordinates):
        dim { dim },
        coordinates { coordinates }
    { }

    VN::VN(const VN& other):
        dim { other.dim },
        coordinates { new real_t[other.dim] }
    {
        copy_n(other.coordinates.get(),
            other.dim,
            coordinates.get());
    }

    VN::VN(VN&& other) noexcept:
        dim { other.dim },
        coordinates { other.coordinates.release() }
    { }

    real_t& VN::operator[](natural_t i)
    {
        return coordinates[i];
    }

    const real_t& VN::operator[](natural_t i) const
    {
        return coordinates[i];
    }

    bool VN::operator==(const VN& other) const
    {
        if (dim != other.dim)
            return false;

        bool are_coordinates_equal { true };
        for (natural_t i = 0; i < dim; i++)
        {
            if (abs(coordinates[i] - other[i]) > EPSILON)
                return false;
        }
        return true;
    }

    VN& VN::operator+=(const VN& other)
    {
        for (natural_t i = 0; i < dim; i++)
        {
            coordinates[i] += other[i];
        }
        return *this;
    }

    VN VN::operator+(const VN& other) const
    {
        VN result { *this };
        return result += other;
    }

    VN& VN::operator-=(const VN& other)
    {
        for (natural_t i = 0; i < dim; i++)
        {
            coordinates[i] -= other[i];
        }
        return *this;
    }

    VN VN::operator-(const VN& other) const
    {
        VN result { *this };
        return result -= other;
    }

    VN& VN::operator*=(real_t k)
    {
        for (natural_t i = 0; i < dim; i++)
        {
            coordinates[i] *= k;
        }
        return *this;
    }

    VN VN::operator*(real_t k) const
    {
        VN result { *this };
        return result *= k;
    }

    real_t VN::mod() const noexcept
    {
        real_t m { 0.0 };
        for (natural_t i = 0; i < dim; i++)
            m += pow(coordinates[i], 2.0);
        return sqrt(m);
    }

    VN& VN::mut_normalize()
    {
        real_t m = mod();
        for (natural_t i = 0; i < dim; i++)
            coordinates[i] /= m;
        return *this;
    }

    VN& VN::mut_coordinate_multiply(const VN& other)
    {
        for (natural_t i = 0; i < dim; i++)
            coordinates[i] *= other[i];
        return *this;
    }

    real_t* VN::rget_data() const noexcept
    {
        return coordinates.get();
    }
}