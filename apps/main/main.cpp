#include "input/input.hpp"
#include "Matrix_t/Matrix_t.hpp" 



int main () try {

    auto data = input_data::get_data<double>();

    Matrices::Matrix_t<double> matrix {data.first, data.first, data.second};
/*
    for (size_t j = 0; j < data.first; j++) {
        for (size_t k = 0; k < data.first; k++) {
            std::cout << matrix[j][k] << " ";
        }
    std::cout << std::endl;
    }
*/
    std::cout << matrix.determinant() << std::endl;
/*
    for (size_t j = 0; j < data.first; j++) {
        for (size_t k = 0; k < data.first; k++) {
            std::cout << matrix[j][k] << " ";
        }
    std::cout << std::endl;
    }
*/


    return EXIT_SUCCESS;
} catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
