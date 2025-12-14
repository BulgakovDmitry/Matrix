#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include "ibuffer.hpp"
#include "buffer.hpp"

namespace matrix {

template<typename T>
class Matrix final {
private:
    std::size_t n_rows_{0};
    std::size_t n_columns_{0};
    std::unique_ptr<IBuffer<T>> data_;

public:
    Matrix() = default;
    Matrix(std::size_t n_rows, std::size_t n_columns) 
        : n_rows_{n_rows}
        , n_columns_{n_columns}
        , data_{std::make_unique<Buffer<T>>(n_rows, n_columns)}
        {}

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    // Matrix(Matirx&& rhs) noexcept {
    //     // TODO
    // }
    // Matrix& operator=(Matrix&& rhs) noexcept {
    //     // TODO
    // }

    /*——————————————————————————————————————— getters ———————————————————————————————————————————*/
    [[nodiscard]] std::size_t get_n_rows() const noexcept { return n_rows_; }
    [[nodiscard]] std::size_t get_n_columns() const noexcept { return n_rows_; }
    [[nodiscard]] std::size_t get_size() const noexcept { return n_rows_ * n_columns_; }
    [[nodiscard]] const T* get_data() noexcept { return data_->get_data(); }
    /*———————————————————————————————————————————————————————————————————————————————————————————*/
};

} // namespace matrix

#endif // INCLUDE_MATRIX_HPP