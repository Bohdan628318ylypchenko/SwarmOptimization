#include "pch.h"

#include "swarm_vn.h"

#include "swarm_numtypes.h"

#include <algorithm>
#include <cmath>
#include <utility>

using std::copy_n;
using std::pow, std::sqrt, std::abs;
using std::move;

namespace swarm::core
{
    VN::VN():
        dim { 0 },
        coordinates { nullptr }
    { }

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
        copy_n(
            other.coordinates,
            other.dim,
            coordinates
        );
    }

    VN::VN(VN&& other) noexcept:
        dim { other.dim },
        coordinates { move(other.coordinates) }
    {
        other.coordinates = nullptr;
    }

    VN::~VN()
    {
        if (coordinates)
            delete[] coordinates;
    }

    real_t* VN::rget_data() const noexcept
    {
        return coordinates;
    }

    natural_t VN::get_dim() const noexcept
    {
        return dim;
    }

    VN& VN::operator=(const VN& other)
    {
        if (this == &other)
            return *this;

        if (coordinates)
            delete[] coordinates;

        coordinates = new real_t[other.dim];
        copy_n(
            other.coordinates,
            other.dim,
            coordinates
        );
        dim = other.dim;

        return *this;
    }

    VN& VN::operator=(VN&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (coordinates)
            delete[] coordinates;

        coordinates = other.coordinates;
        dim = other.dim;
        other.coordinates = nullptr;

        return *this;
    }

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

    real_t VN::mod() const noexcept
    {
        real_t m { 0.0 };
        for (natural_t i = 0; i < dim; i++)
            m += pow(coordinates[i], 2.0);
        return sqrt(m);
    }

    VN& VN::mut_add(const VN& other)
    {
        for (natural_t i = 0; i < dim; i++)
        {
            coordinates[i] += other[i];
        }
        return *this;
    }

    VN& VN::mut_subtract(const VN& other)
    {
        for (natural_t i = 0; i < dim; i++)
        {
            coordinates[i] -= other[i];
        }
        return *this;
    }

    VN& VN::mut_multiply_on_scalar(real_t k)
    {
        for (natural_t i = 0; i < dim; i++)
        {
            coordinates[i] *= k;
        }
        return *this;
    }

    VN& VN::mut_coordinate_multiply(const VN& other)
    {
        for (natural_t i = 0; i < dim; i++)
            coordinates[i] *= other[i];
        return *this;
    }

    VN& VN::mut_normalize()
    {
        real_t m = mod();
        for (natural_t i = 0; i < dim; i++)
            coordinates[i] /= m;
        return *this;
    }

    VN VN::copy() const
    {
        VN result { dim, new real_t[dim] };

        copy_n(
            coordinates,
            result.dim,
            result.coordinates
        );

        return result;
    }
}