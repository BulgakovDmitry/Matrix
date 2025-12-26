#include <cstddef>
#include <iostream>
#include "matrix.hpp"
#include "dump.hpp"

int main() {
    std::size_t M;
    std::size_t N;
    std::cin >> M >> N;
    matrix::Matrix<int> matrix(M, N);

    int v = 0;
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            std::cin >> v;
            matrix.insert(i, j, v); 
        }
    }
    matrix::matrix_dump<int>(matrix, std::cout);

}