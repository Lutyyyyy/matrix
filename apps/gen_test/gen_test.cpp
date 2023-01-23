#include "Matrix_t/Matrix_t.hpp"
#include <fstream>

int main () {

    std::ofstream mixed_buffer_file;
    mixed_buffer_file.open ("gen_test_errors.txt", std::ios::trunc);
    
    size_t size = 100;
    size_t nMatrices = 200;

    for (size_t i = 0; i != nMatrices; i++) {
        
        std::vector<double> vec (size * size, 0);
        for (size_t j = 0; j < size; j++) {
            for (size_t k = 0; k < size; k++) {
                if (k == 0 && j == 0) {
                    vec[0] = 42;
                } else if (j <= k) {
                    vec[size * j + k] = 1;
                } else {
                    vec[size * j + k] = 0;
                }
            }
        }
        Matrices::Matrix_t<double> mtx {size, size, vec};
        Matrices::Matrix_t<double> mtx_copy = mtx;

        for (size_t nAdds = 0; nAdds < 100; nAdds++) {
            mtx.add_rows( rand() % 100, rand() % 100);
        }

        size_t nSwaps = 0;
        while (nSwaps < 100) {
            bool is_swap = mtx.swap_rows(rand() % 100, rand() % 100);
            if (is_swap)
                nSwaps++;    
        }

        for (size_t nSubs = 0; nSubs < 100; nSubs++) {
            mtx.subtract_rows( rand() % 100, rand() % 100);
        }

        double res = mtx.determinant();

        if ( !Matrices::equal_eps(res, 42.0) ) {
            std::cout << "TEST #" << i << " FAILED\t\tOUTPUT: " << res << std::endl;
            

            mixed_buffer_file << "TEST #" << i << " FAILED\nINPUT: " << std::endl << std::endl;
            for (size_t j = 0; j < size; j++) {
                for (size_t k = 0; k < size; k++) {
                    mixed_buffer_file << vec[j*size+k] << " ";
                }
            mixed_buffer_file << std::endl;
            }
            mixed_buffer_file << std::endl << std::endl;


            mixed_buffer_file << "AFTER MIXING:"  << std::endl;
            for (size_t j = 0; j < size; j++) {
                for (size_t k = 0; k < size; k++) {
                    mixed_buffer_file << mtx[j][k] << " ";
                }
            mixed_buffer_file << std::endl;
            }
            mixed_buffer_file << std::endl << std::endl;

            mixed_buffer_file << "OUTPUT: " << res << std::endl << "===========================================" << std::endl;
        }
        
    }

    mixed_buffer_file.close();
    return 0;
}

