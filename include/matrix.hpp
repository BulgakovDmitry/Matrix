#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include "buffer.hpp"

namespace matrix {

template<typename T>
class Matrix final {
private:
    std::size_t n_rows_{0};
    std::size_t n_columns_{0};
    Buffer<T> data_;

public:
    Matrix() = default;
    Matrix(std::size_t n_rows, std::size_t n_columns) 
    : n_rows_{n_rows}
    , n_columns_{n_columns}
    , data_(n_rows, n_columns)
    {}

};

} // namespace matrix

#endif // INCLUDE_MATRIX_HPP