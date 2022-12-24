#include "Matrix_t/Matrix_t.hpp"
#include "gtest/gtest.h"


namespace Matrices
{

namespace testing
{

namespace class_testing
{

namespace Buffer_t_class_testing
{

TEST (ctor_test, default_ctors)
{ 
    Buffer_t<int> dt1 {10, 21};
    EXPECT_EQ (dt1.rows(), 10);
    EXPECT_EQ (dt1.columns(), 21);
}

TEST (ctor_test, move_ctor)
{  
    Buffer_t<int> dt1_move = std::move(Buffer_t<int> {10, 21}); 
    EXPECT_EQ (dt1_move.rows(), 10);
    EXPECT_EQ (dt1_move.columns(), 21);
}

TEST (assignment_test, move_assignment)
{ 
    Buffer_t<int> dt1 = Buffer_t<int> {2, 2}; 
    EXPECT_EQ (dt1.rows(), 2);
    EXPECT_EQ (dt1.columns(), 2);
}

TEST (ctor_test, empty_buffer_ctor)
{ 
    Buffer_t<int> dt {};
    EXPECT_EQ (dt.rows(), 0);
    EXPECT_EQ (dt.columns(), 0);
    EXPECT_EQ (dt.buffer(), nullptr);
    EXPECT_EQ (dt.buf_size(), 0);
}

TEST (ctor_test, zero_size_ctor)
{ 
    Buffer_t<int> dt {0, 0};
    EXPECT_EQ (dt.rows(), 0);
    EXPECT_EQ (dt.columns(), 0);
    EXPECT_EQ (dt.buffer(), nullptr);
    EXPECT_EQ (dt.buf_size(), 0);
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