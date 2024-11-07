#include "pch.h"

#include "CppUnitTest.h"
#include "swarm_rndgen.h"
#include "swarm_numtypes.h"

#include <string>
#include <stdexcept>
#include <system_error>

#ifdef _DEBUG
#include "memory_leak.h"
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using swarm::core::FileRandomNumbersGenerator;

using std::error_code;
using std::wstring;
using std::runtime_error;

namespace swarm::test
{
    TEST_CLASS(FileRandomNumbersGeneratorTests)
    {
    private:
        static constexpr size_t RANDOM_NUMBERS_COUNT { 4505600 };
        static wstring RANDOM_NUMBERS_SOURCE_FILENAME;

        static constexpr natural_t DIM { 1000 };

#ifdef _DEBUG
        static MemoryLeak memory_leak;
#endif

    public:
#ifdef _DEBUG
        TEST_CLASS_INITIALIZE(initialize_vn_tests)
        {
            memory_leak.checkpoint();
        }

        TEST_CLASS_CLEANUP(cleanup_vn_tests)
        {
            memory_leak.compare();
        }
#endif
        TEST_METHOD(initialize_random_numbers_source_fail)
        {
            error_code result
            {
                FileRandomNumbersGenerator::initialize_random_numbers_source(L"non-existent-file.rnd")
            };

            if (!result) Assert::Fail();

            FileRandomNumbersGenerator::free_random_numbers_source();
        }

        TEST_METHOD(initialize_random_numbers_source_success)
        {
            error_code result
            {
                FileRandomNumbersGenerator::initialize_random_numbers_source(
                    RANDOM_NUMBERS_SOURCE_FILENAME
                )
            };

            Assert::AreEqual(0, result.value());
            Assert::IsTrue(nullptr != FileRandomNumbersGenerator::rget_data());
            Assert::AreEqual(
                RANDOM_NUMBERS_COUNT, FileRandomNumbersGenerator::rget_random_numbers_count()
            );

            FileRandomNumbersGenerator::free_random_numbers_source();
        }

        TEST_METHOD(invalid_initial_pos)
        {
            FileRandomNumbersGenerator::initialize_random_numbers_source(
                RANDOM_NUMBERS_SOURCE_FILENAME
            );

            Assert::ExpectException<runtime_error, FileRandomNumbersGenerator>(
                []() -> FileRandomNumbersGenerator
                {
                    FileRandomNumbersGenerator generator { 5000000000 };
                    return generator;
                }
            );

            FileRandomNumbersGenerator::free_random_numbers_source();
        }

        TEST_METHOD(next_n_uniform_double_1_copy)
        {
            FileRandomNumbersGenerator::initialize_random_numbers_source(
                RANDOM_NUMBERS_SOURCE_FILENAME
            );

            FileRandomNumbersGenerator generator { 0 };

            real_t actual[DIM];
            generator.next_n_uniform_double(DIM, actual);

            real_t* expected_data = FileRandomNumbersGenerator::rget_data();
            for (natural_t i = 0; i < DIM; i++)
            {
                Assert::AreEqual(expected_data[i], actual[i]);
            }

            FileRandomNumbersGenerator::free_random_numbers_source();
        }

        TEST_METHOD(next_n_uniform_double_2_copies)
        {
            constexpr size_t shift { 501 };

            FileRandomNumbersGenerator::initialize_random_numbers_source(
                RANDOM_NUMBERS_SOURCE_FILENAME
            );

            FileRandomNumbersGenerator generator { RANDOM_NUMBERS_COUNT - shift };

            real_t actual[DIM];
            generator.next_n_uniform_double(DIM, actual);

            real_t* expected_data = FileRandomNumbersGenerator::rget_data();
            for (natural_t i = 0; i < shift; i++)
            {
                Assert::AreEqual(expected_data[RANDOM_NUMBERS_COUNT - shift + i], actual[i]);
            }
            for (natural_t i = shift; i < DIM; i++)
            {
                Assert::AreEqual(expected_data[i - shift], actual[i]);
            }
            Assert::AreEqual((size_t)499, generator.get_pos());

            FileRandomNumbersGenerator::free_random_numbers_source();
        }

        TEST_METHOD(next_n_uniform_double_range)
        {
            constexpr size_t shift { 456 };
            constexpr real_t min { -100 };
            constexpr real_t max { 100 };

            FileRandomNumbersGenerator::initialize_random_numbers_source(
                RANDOM_NUMBERS_SOURCE_FILENAME
            );

            FileRandomNumbersGenerator generator { RANDOM_NUMBERS_COUNT - shift };
            real_t actual[DIM];
            generator.next_n_uniform_double(DIM, actual, min, max);

            for (natural_t i = 0; i < DIM; i++)
            {
                Assert::IsTrue(min <= actual[i] && actual[i] <= max);
            }
            Assert::AreEqual((size_t)544, generator.get_pos());

            FileRandomNumbersGenerator::free_random_numbers_source();
        }

#ifdef NDEBUG
        TEST_METHOD(next_uniform_double)
        {
            FileRandomNumbersGenerator::initialize_random_numbers_source(
                RANDOM_NUMBERS_SOURCE_FILENAME
            );

            FileRandomNumbersGenerator generator { 0 };

            real_t* expected_data = FileRandomNumbersGenerator::rget_data();
            for (auto t = 0; t < 2; t++)
            {
                for (size_t i = 0; i < RANDOM_NUMBERS_COUNT; i++)
                {
                    Assert::AreEqual(
                        expected_data[i],
                        generator.next_uniform_double()
                    );
                }
            }

            FileRandomNumbersGenerator::free_random_numbers_source();
        }
#endif

    };

    wstring FileRandomNumbersGeneratorTests::RANDOM_NUMBERS_SOURCE_FILENAME
    {
        L"normalized-quantum-big.rnd"
    };
#ifdef _DEBUG
    MemoryLeak FileRandomNumbersGeneratorTests::memory_leak { L"swarm_rndgen_tests" };
#endif

}