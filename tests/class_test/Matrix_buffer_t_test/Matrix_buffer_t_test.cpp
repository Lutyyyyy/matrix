#include "Matrix_t/Matrix_t.hpp"
#include "gtest/gtest.h"


namespace Matrices
{

namespace testing
{

namespace class_testing
{

namespace Matrix_buffer_t_class_testing
{

TEST (ctor_test, default_ctors)
{ 
    Matrix_buffer_t<int> dt{100, 100};
    EXPECT_EQ (dt.used(), 0);
    EXPECT_EQ (dt.buffer_size(), 10000);
    EXPECT_EQ (dt.rows(), 100);
    EXPECT_EQ (dt.columns(), 100);
}

TEST (ctor_test, move_ctor)
{  
    Matrix_buffer_t<int> dt = std::move(Matrix_buffer_t<int>{10, 20}); 
    EXPECT_EQ (dt.used(), 0);
    EXPECT_EQ (dt.buffer_size(), 200);
    EXPECT_EQ (dt.rows(), 10);
    EXPECT_EQ (dt.columns(), 20);
}

TEST (assignment_test, move_assignment)
{ 
    Matrix_buffer_t<int> dt = Matrix_buffer_t<int>(10, 10); 
    EXPECT_EQ (dt.used(), 0);
    EXPECT_EQ (dt.buffer_size(), 100);
    EXPECT_EQ (dt.rows(), 10);
    EXPECT_EQ (dt.columns(), 10);
}

TEST (ctor_test, zero_size_ctor)
{ 
    Matrix_buffer_t<int> dt{0, 0};
    EXPECT_EQ (dt.used(), 0);
    EXPECT_EQ (dt.buffer_size(), 0);
    EXPECT_EQ (dt.rows(), 0);
    EXPECT_EQ (dt.columns(), 0);
}

TEST (ctor_test, vector_values_init)
{ 
    std::vector<int> vec = {1, 0, 0, 0, 1, 0};
    Matrix_buffer_t<int> mtx_buf = {2, 3, vec};
    
    EXPECT_EQ (mtx_buf.used(), 6);
    EXPECT_EQ (mtx_buf.buffer_size(), 6);
    EXPECT_EQ (mtx_buf.rows(), 2);
    EXPECT_EQ (mtx_buf.columns(), 3);

    EXPECT_EQ (equal_eps (mtx_buf[0], 1.0), true);
    EXPECT_EQ (equal_eps (mtx_buf[1], 0.0), true);
    EXPECT_EQ (equal_eps (mtx_buf[2], 0.0), true);
    EXPECT_EQ (equal_eps (mtx_buf[3], 0.0), true);
    EXPECT_EQ (equal_eps (mtx_buf[4], 1.0), true);
    EXPECT_EQ (equal_eps (mtx_buf[5], 0.0), true);

}

} //Buffer_t_class_testing
} //class_testing
} //testing
} //Matrices

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS();
}