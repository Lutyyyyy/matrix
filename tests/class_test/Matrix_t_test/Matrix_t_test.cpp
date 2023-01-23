#include "Matrix_t/Matrix_t.hpp"
#include "gtest/gtest.h"


namespace Matrices
{

namespace testing
{

namespace class_testing
{

namespace Matrix_t_class_testing
{

TEST (methods_test, swap_test)
{
    std::vector<int> vec = {1, 0, 0, 0, 1, 0};
    Matrix_t<int> mtx = {2, 3, vec};
    EXPECT_EQ (mtx.rows(), 2);
    EXPECT_EQ (mtx.columns(), 3);
    
    mtx.swap_rows(0, 1);
    
    EXPECT_EQ (equal_eps (mtx[0][0], 0.0), true);
    EXPECT_EQ (equal_eps (mtx[0][1], 1.0), true);
    EXPECT_EQ (equal_eps (mtx[0][2], 0.0), true);
    EXPECT_EQ (equal_eps (mtx[1][0], 1.0), true);
    EXPECT_EQ (equal_eps (mtx[1][1], 0.0), true);
    EXPECT_EQ (equal_eps (mtx[1][2], 0.0), true);

    Matrix_t<int> mtx2 = {3, 2, vec};
    EXPECT_EQ (mtx.rows(), 2);
    EXPECT_EQ (mtx.columns(), 3);

    mtx2.swap_columns(1, 0);

    EXPECT_EQ (equal_eps (mtx2[0][0], 0.0), true);
    EXPECT_EQ (equal_eps (mtx2[0][1], 1.0), true);
    EXPECT_EQ (equal_eps (mtx2[1][0], 0.0), true);
    EXPECT_EQ (equal_eps (mtx2[1][1], 0.0), true);
    EXPECT_EQ (equal_eps (mtx2[2][0], 0.0), true);
    EXPECT_EQ (equal_eps (mtx2[2][1], 1.0), true);    
}

} //Matrix_t_class_testing
} //class_testing
} //testing
} //Matrices

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}

