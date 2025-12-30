#include <cstddef>
#include <iostream>
#include "matrix.hpp"
#include "dump.hpp"
#include "driver.hpp"

void driver() {
    std::size_t M;
    std::cin >> M;
    
    matrix::Matrix<double> matrix(M, M);

    double v = 4;
    
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            std::cin >> v;
            try {
                matrix.insert(i, j, v); 
            } catch(std::runtime_error& e) {
                std::cerr << e.what();
            }
        }
    }
    // matrix::matrix_dump<double>(matrix, std::cout);
    try {
        double determinant = matrix.calculate_determinant();
        std::cout << /*"determinant = " <<*/ determinant << '\n';
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
    }
}