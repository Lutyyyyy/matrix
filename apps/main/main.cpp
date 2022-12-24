#include "input/input.hpp"
#include "Matrix_t/Matrix_t.hpp" 



int main () {

    auto data = input_data::get_data<double>();

    Matrices::Matrix_t<double> matrix {data.first, data.first, data.second};
    
    double res = matrix.determinant();
    
    std::cout << res << std::endl;

    return 0;
}
