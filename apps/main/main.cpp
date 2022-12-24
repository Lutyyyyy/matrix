#include "input/input.hpp"
#include "Matrix_t/Matrix_t.hpp" 



int main () try {

    auto data = input_data::get_data<double>();

    Matrices::Matrix_t<double> matrix {data.first, data.first, data.second};
    
    double res = matrix.determinant();
    
    std::cout << res << std::endl;

    return EXIT_SUCCESS;
} catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
