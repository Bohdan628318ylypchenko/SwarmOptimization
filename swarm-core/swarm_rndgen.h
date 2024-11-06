#pragma once

#include "swarm_numtypes.h"

#include <string>
#include <system_error>

namespace swarm::core
{
    class FileRandomNumbersGenerator
    {
    private:
        static size_t random_numbers_count;
        static real_t* random_numbers_source;

        size_t pos;

    public:
        static std::error_code initialize_random_numbers_source(
            const std::wstring& source_filename
        );
        static void free_random_numbers_source();
        static real_t* rget_data() noexcept;
        static size_t rget_random_numbers_count() noexcept;

        FileRandomNumbersGenerator(size_t initial_pos);

        size_t get_pos() const noexcept;

        real_t next_uniform_double() noexcept;

        real_t* next_n_uniform_double(natural_t n, real_t* dst) noexcept;
        real_t* next_n_uniform_double(
            natural_t n, real_t* dst,
            real_t min, real_t max
        ) noexcept;
    };
}
