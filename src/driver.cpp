#include <cstddef>
#include <iostream>
#include "matrix/matrix.hpp"
#include "dump.hpp"
#include "driver.hpp"

void driver() {
    std::size_t N;

    std::cin >> N;
    
    matrix::Matrix<double> matrix(N, N);

    double value;
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            std::cin >> value;
            try {
                matrix.insert(i, j, value); 
            } catch(std::runtime_error& e) {
                std::cerr << e.what();
            }
        }
    }

    try {
        double determinant = matrix.calculate_determinant();
        std::cout << /*"determinant = " <<*/ determinant << '\n';
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
    }
}