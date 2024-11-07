#include "pch.h"

#include "CppUnitTest.h"
#include "swarm_vn.h"
#include "swarm_numtypes.h"

#ifdef _DEBUG
#include "memory_leak.h"
#endif

#include <memory>
#include <cstring>
#include <utility>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using swarm::core::VN;

using std::unique_ptr, std::make_unique;
using std::move;

using std::memcpy;

namespace swarm::test
{
    TEST_CLASS(VNTests)
    {
    private:
#ifdef _DEBUG
        static MemoryLeak memory_leak;
#endif

        inline real_t* copy_coordinates(
            const real_t* const coordinates,
            natural_t dim) const
        {
            return static_cast<real_t*>(
                memcpy(
                    new real_t[dim],
                    coordinates,
                    dim * sizeof(real_t)
                )
            );
        }

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
        TEST_METHOD(copy_constructor)
        {
            constexpr natural_t DIM { 4 };
            real_t* coordinates = new real_t[DIM] { 5, 6, 7, 8 };
            VN vn1 { DIM, coordinates };
            VN vn2 { vn1 };

            Assert::AreEqual(DIM, vn1.get_dim());
            Assert::AreEqual(DIM, vn2.get_dim());

            for (natural_t i = 0; i < DIM; i++)
            {
                Assert::AreEqual(coordinates[i], vn1[i]);
                Assert::AreEqual(coordinates[i], vn2[i]);
            }

            vn2[1] = 100.0;

            Assert::AreEqual(coordinates[1], vn1[1]);
            Assert::AreEqual(100.0, vn2[1]);
        }

        TEST_METHOD(move_constructor)
        {
            constexpr natural_t DIM { 4 };
            real_t* coordinates = new real_t[DIM] { 5, 6, 7, 8 };
            VN vn1 { DIM, coordinates };
            VN vn2 { move(vn1) };

            Assert::AreEqual(DIM, vn2.get_dim());

            for (natural_t i = 0; i < DIM; i++)
            {
                Assert::AreEqual(coordinates[i], vn2[i]);
            }
        }

        TEST_METHOD(equal_true_operator)
        {
            constexpr natural_t DIM { 4 };
            VN vn1 { DIM, new real_t[DIM] { 1, 2, 3, 4 } };
            VN vn2 { DIM, new real_t[DIM] { 1.0001, 2, 3, 4 } };

            Assert::IsTrue(vn1 == vn2);
        }

        TEST_METHOD(equal_false_operator)
        {
            constexpr natural_t DIM { 4 };
            VN vn1 { DIM, new real_t[DIM] { 1, 2, 3, 4 } };
            VN vn2 { DIM, new real_t[DIM] { 1.00011, 2, 3, 4 } };

            Assert::IsFalse(vn1 == vn2);
        }

        TEST_METHOD(mut_add)
        {
            constexpr natural_t DIM { 2 };
            constexpr real_t c2[DIM] { 3, 4 };

            VN vn1 { DIM, new real_t[DIM] { 1, 2 } };
            VN vn2 { DIM, copy_coordinates(c2, DIM) };

            vn1.mut_add(vn2);

            VN expected_vn1 { DIM, new real_t[DIM] { 4, 6 } };
            Assert::IsTrue(vn1 == expected_vn1);

            VN expected_vn2 { DIM, copy_coordinates(c2, DIM) };
            Assert::IsTrue(vn2 == expected_vn2);
        }

        TEST_METHOD(mut_add_copy)
        {
            constexpr natural_t DIM { 2 };
            constexpr real_t c1[DIM] { 1, 2 };
            constexpr real_t c2[DIM] { 3, 4 };

            VN vn1 { DIM, copy_coordinates(c1, DIM) };
            VN vn2 { DIM, copy_coordinates(c2, DIM) };

            VN vn3 { vn1 };
            vn3.mut_add(vn2);

            VN expected_vn1 { DIM, copy_coordinates(c1, DIM) };
            Assert::IsTrue(vn1 == expected_vn1);

            VN expected_vn2 { DIM, copy_coordinates(c2, DIM) };
            Assert::IsTrue(vn2 == expected_vn2);

            VN expected_vn3 { DIM, new real_t[DIM] { 4, 6 } };
            Assert::IsTrue(vn3 == expected_vn3);
        }

        TEST_METHOD(mut_subtract)
        {
            constexpr natural_t DIM { 2 };
            const real_t c2[DIM] { 3, 4 };

            VN vn1 { DIM, new real_t[DIM] { 1, 2 } };
            VN vn2 { DIM, copy_coordinates(c2, DIM) };

            vn1.mut_subtract(vn2);

            VN expected_vn1 { DIM, new real_t[DIM] { -2, -2 } };
            Assert::IsTrue(vn1 == expected_vn1);

            VN expected_vn2 { DIM, copy_coordinates(c2, DIM) };
            Assert::IsTrue(vn2 == expected_vn2);
        }

        TEST_METHOD(mut_subtract_copy)
        {
            constexpr natural_t DIM { 2 };
            const real_t c1[DIM] { 1, 2 };
            const real_t c2[DIM] { 3, 4 };

            VN vn1 { DIM, copy_coordinates(c1, DIM) };
            VN vn2 { DIM, copy_coordinates(c2, DIM) };

            VN vn3 { vn1 };
            vn3.mut_subtract(vn2);

            VN expected_vn1 { DIM, copy_coordinates(c1, DIM) };
            Assert::IsTrue(vn1 == expected_vn1);

            VN expected_vn2 { DIM, copy_coordinates(c2, DIM) };
            Assert::IsTrue(vn2 == expected_vn2);

            VN expected_vn3 { DIM, new real_t[DIM] { -2, -2 } };
            Assert::IsTrue(vn3 == expected_vn3);
        }

        TEST_METHOD(mut_multiply_on_scalar)
        {
            constexpr natural_t DIM { 2 };

            VN vn { DIM, new real_t[DIM] { 5, 2 } };

            vn.mut_multiply_on_scalar(3.0);

            VN expected_vn { DIM, new real_t[DIM] { 15, 6 } };
            Assert::IsTrue(vn == expected_vn);
        }

        TEST_METHOD(mod)
        {
            constexpr natural_t DIM { 2 };
            VN vn { DIM, new real_t[DIM] { 3, 4 } };

            Assert::AreEqual(5.0, vn.mod());
        }

        TEST_METHOD(mut_normalize)
        {
            constexpr natural_t DIM { 2 };
            VN vn { DIM, new real_t[DIM] { 3, 4 } };

            VN expected_vn { DIM, new real_t[DIM] { 0.6, 0.8 } };
            Assert::IsTrue(vn.mut_normalize() == expected_vn);
        }

        TEST_METHOD(mut_coordinate_multiply)
        {
            constexpr natural_t DIM { 3 };
            const real_t c2[DIM] { 5, 6, 7 };

            VN vn1 { DIM, new real_t[DIM] { 2, 3, 4 } };
            VN vn2 { DIM, copy_coordinates(c2, DIM) };

            VN expected_vn1 { DIM, new real_t[DIM] { 10, 18, 28 } };
            Assert::IsTrue(vn1.mut_coordinate_multiply(vn2) == expected_vn1);

            VN expected_vn2 { DIM, copy_coordinates(c2, DIM) };
            Assert::IsTrue(vn2 == expected_vn2);
        }
    };

#ifdef _DEBUG
    MemoryLeak VNTests::memory_leak { L"swarm_vn_tests" };
#endif

}