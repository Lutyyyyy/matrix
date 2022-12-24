#include "Matrix_t/Matrix_t.hpp"
#include "gtest/gtest.h"


namespace Matrices
{

namespace testing
{

namespace algorithm_testing
{   

namespace determinant_computing_algorithm_testing
{

TEST (determinant_computing_test, general)
{

    std::vector<int> vec0(9, 0);
    EXPECT_EQ ( equal_eps ( (Matrix_t<int>{3, 3, vec0}).determinant(), 0), true );

    std::vector<int> vec1(9, 0);
    EXPECT_EQ ( equal_eps ( (Matrix_t<int>{3, 3, vec1}).determinant(), 0), true );

    std::vector<int> vec2 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ ( equal_eps ( (Matrix_t<int>{3, 3, vec2}).determinant(), 0), true );

    std::vector<int> vec3 = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    EXPECT_EQ ( (Matrix_t<int>{3, 3, vec3}).determinant(), 27 );

    std::vector<double> vec4 = {1.5, -0.1, 2.0, 4.21, 0.0, 0.25, 10, 1.0, -3.0};
    EXPECT_EQ ( (Matrix_t<double>{3, 3, vec4}).determinant(), 6.532 );

    Matrix_t<int> zero_mtx {200, 200, 0};
    EXPECT_EQ ( equal_eps (zero_mtx.determinant(), 0), true );

    Matrix_t<int> value_mtx {200, 200, 200};
    EXPECT_EQ ( equal_eps (value_mtx.determinant(), 0), true );

}

} //determinant_computing_testing
} //algorithm_testing
} //testing
} //Matrices

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}


