#include <cstddef>
#include <iostream>
#include "matrix/matrix.hpp"
#include "dump.hpp"
#include "driver.hpp"

void driver() {
    std::size_t M;
    std::size_t N;

    std::cin >> M >> N;
    
    matrix::Matrix<double> matrix(M, N);

    double v = 4;
    
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            std::cin >> v;
            try {
                matrix.insert(i, j, v); 
            } catch(std::runtime_error& e) {
                std::cerr << e.what();
            }
        }
    }
    
    matrix::matrix_dump<double>(matrix, std::cout);
    matrix.swap_columns(1, 2);
    matrix::matrix_dump<double>(matrix, std::cout);
    matrix.swap_rows(1, 2);
    matrix::matrix_dump<double>(matrix, std::cout);
    
    // try {
    //     double determinant = matrix.calculate_determinant();
    //     std::cout << /*"determinant = " <<*/ determinant << '\n';
    // } catch (std::runtime_error& e) {
    //     std::cerr << e.what();
    // }
}