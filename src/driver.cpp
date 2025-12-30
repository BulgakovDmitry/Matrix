#include <cstddef>
#include <iostream>
#include "matrix.hpp"
#include "dump.hpp"
#include "driver.hpp"

void driver() {
    std::size_t M;
    std::cin >> M;
    
    matrix::Matrix<int> matrix(M, M);

    int v = 4;
    
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
    matrix::matrix_dump<int>(matrix, std::cout);

    try {
        int determinant = matrix.calculate_determinant();
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
    }
}