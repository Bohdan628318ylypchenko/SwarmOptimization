#include "pch.h"

#include "swarm_rndgen.h"

#include "swarm_numtypes.h"

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <system_error>

using std::wstring, std::string, std::to_string;
using std::ifstream;
using std::cerr;
using std::runtime_error, std::error_code;
using std::memcpy;

namespace swarm::core
{
    size_t FileRandomNumbersGenerator::random_numbers_count { 0 };

    real_t* FileRandomNumbersGenerator::random_numbers_source { };

    real_t FileRandomNumbersGenerator::map_to_segment_from01(
        real_t r, real_t min, real_t max
    ) {
        return min + r * (max - min);
    }

    error_code FileRandomNumbersGenerator::initialize_random_numbers_source(
        const std::wstring& source_filename
    ) {
        try
        {
            ifstream source
            {
                source_filename,
                std::ios::in | std::ios::binary
            };

            source.exceptions(ifstream::failbit | ifstream::badbit);

            source.read(
                reinterpret_cast<char*>(&random_numbers_count),
                sizeof(size_t)
            );

            random_numbers_source = new real_t[random_numbers_count];
            source.read(
                reinterpret_cast<char*>(random_numbers_source),
                random_numbers_count * sizeof(real_t)
            );
        }
        catch (const std::ios_base::failure& e)
        {
            cerr <<
                "Error: failed to initialize random numbers source\n" <<
                "Cause: " << e.what() << '\n' <<
                "Error code: " << e.code() << '\n';
            return e.code();
        }
        return { };
    }

    void FileRandomNumbersGenerator::free_random_numbers_source()
    {
        if (random_numbers_source)
        {
            delete[] random_numbers_source;
            random_numbers_source = nullptr;
            random_numbers_count = 0;
        }
    }

    real_t* FileRandomNumbersGenerator::rget_data() noexcept
    {
        return random_numbers_source;
    }

    size_t FileRandomNumbersGenerator::rget_random_numbers_count() noexcept
    {
        return random_numbers_count;
    }

    FileRandomNumbersGenerator::FileRandomNumbersGenerator(size_t initial_pos):
        pos { initial_pos }
    {
        if (pos >= random_numbers_count)
        {
            string msg {
                "invalid initial pos: " + to_string(pos) + "; " +
                "random numbers count: " + to_string(random_numbers_count) + "\n"
            };

            throw runtime_error(msg);
        }
    }

    size_t FileRandomNumbersGenerator::get_pos() const noexcept
    {
        return pos;
    }

    real_t FileRandomNumbersGenerator::next_uniform_double() noexcept
    {
        real_t result { random_numbers_source[pos] };
        pos = (pos + 1) % random_numbers_count;
        return result;
    }

    real_t FileRandomNumbersGenerator::next_uniform_double(real_t min, real_t max) noexcept
    {
        return map_to_segment_from01(next_uniform_double(), min, max);
    }

    real_t* FileRandomNumbersGenerator::next_n_uniform_double(natural_t n, real_t* dst) noexcept
    {
        size_t diff { random_numbers_count - pos };
        if (diff >= n)
        {
            memcpy(dst, random_numbers_source + pos, n * sizeof(size_t));
            pos += n;
            return dst;
        }
        else
        {
            memcpy(dst, random_numbers_source + pos, diff * sizeof(size_t));
            pos = n - diff;
            memcpy(dst + diff, random_numbers_source, pos * sizeof(size_t));
            return dst;
        }
    }

    real_t* FileRandomNumbersGenerator::next_n_uniform_double(
        natural_t n, real_t* dst,
        real_t min, real_t max
    ) noexcept {
        next_n_uniform_double(n, dst);
        for (natural_t i = 0; i < n; i++)
            dst[i] = map_to_segment_from01(dst[i], min, max);
        return dst;
    }
}