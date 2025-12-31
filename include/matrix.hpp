#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include "ibuffer.hpp"
#include "buffer.hpp"
#include "imatrix.hpp"
#include "cmp.hpp"
#include <utility>

namespace matrix {

template<std::floating_point T>
class Matrix final : public IMatrix<T> {
private:
    std::size_t n_rows_{0};
    std::size_t n_columns_{0};
    std::unique_ptr<IBuffer<T>> data_;

public:
    Matrix() = default;

    Matrix(std::size_t n_rows, std::size_t n_columns) 
        : n_rows_{n_rows}
        , n_columns_{n_columns}
        , data_{std::make_unique<Buffer<T>>(n_rows_, n_columns_)}
        {}

    Matrix(std::size_t size)
        : n_rows_{size}
        , n_columns_{size}
        , data_{std::make_unique<Buffer<T>>(n_rows_, n_columns_)}
        {}

    Matrix(const Matrix& rhs) 
        : n_rows_(rhs.n_rows_)
        , n_columns_(rhs.n_columns_)
        , data_(rhs.data_ ? rhs.data_->clone() : nullptr)
        {}

    Matrix& operator=(const Matrix& rhs) {
        if (this == std::addressof(rhs)) return rhs;

        Matrix tmp(rhs);
        swap(tmp);
        return *this;
    }

    Matrix(Matrix&& rhs) noexcept {
        swap(rhs); 
    }

    Matrix& operator=(Matrix&& rhs) noexcept {
        swap(rhs); 
        return this;
    }

    /*——————————————————————————————————————— IMatrix ———————————————————————————————————————————*/
    ~Matrix() override = default;
    
    [[nodiscard]] std::size_t get_n_rows()    const noexcept { return n_rows_; }
    [[nodiscard]] std::size_t get_n_columns() const noexcept { return n_columns_; }
    [[nodiscard]] std::size_t get_size()      const noexcept { return n_rows_ * n_columns_; }

    [[nodiscard]] const T* get_data() const noexcept { return data_->get_data(); }
    [[nodiscard]]       T* get_data()       noexcept { return data_->get_data(); }
    /*———————————————————————————————————————————————————————————————————————————————————————————*/

    [[nodiscard]] bool is_square() const noexcept { return n_rows_ == n_columns_; }

    T calculate_determinant() const {
        if (!is_square()) {
            throw std::runtime_error("it is impossible to calculate the determinant of a non-square matrix\n");
        }

        if (n_rows_ == 0) return T(1);
        if (n_rows_ == 1) return *(data_->get_data());

        Matrix tmp(*this);

        T* data = tmp.data_->get_data();

        int sign = 1;
        for (std::size_t k = 0; k < n_rows_; ++k) {
            std::size_t p = tmp.find_row_with_max_abs_value_in_column(k);

            if (cmp::is_zero(data[k * n_rows_ + p])) {
                return T(0);
            }

            // swap rows => меняется знак det
            if (p != k) {
                tmp.swap_rows(k, p);
                sign = -sign;
            }

            const T pivot = data[k * n_rows_ + k];
            if (cmp::is_zero(pivot)) {
                return T(0);
            }

            // зануляем элементы ниже диагонали
            for (std::size_t i = k + 1; i < n_rows_; ++i) {
                const T factor = data[i * n_rows_ + k] / pivot;
                data[i * n_rows_ + k] = T(0);
                for (std::size_t j = k + 1; j < n_rows_; ++j) {
                    data[i * n_rows_ + j] -= factor * data[k * n_rows_ + j];
                }
            }
        }

        T det = (sign > 0) ? T(1) : T(-1);
        for (std::size_t i = 0; i < n_rows_; ++i) {
            det *= data[i * n_rows_ + i];
        }
        return det;
    }

    void swap_rows(std::size_t row_1, std::size_t row_2) {
        if (row_1 == row_2) return;
        if (row_1 >= n_rows_ || row_2 >= n_rows_) {
            throw std::out_of_range("swap_rows: row index out of range");
        }

        T* data = data_->get_data();
        for (std::size_t j = 0; j < n_columns_; ++j) {
            std::swap(data[row_1 * n_columns_ + j], 
                      data[row_2 * n_columns_ + j]);
        }
    }

    void swap_columns(std::size_t col_1, std::size_t col_2) {
        if (col_1 == col_2) return;
        if (col_1 >= n_columns_ || col_2 >= n_columns_) {
            throw std::out_of_range("swap_columns: column index out of range");
        }

        T* data = data_->get_data();
        for (std::size_t i = 0; i < n_rows_; ++i) {
            std::swap(data[i * n_columns_ + col_1],
                      data[i * n_columns_ + col_2]);
        }
    }

    void insert(std::size_t i, std::size_t j, const T& value) {
        if (i >= n_rows_ || j >= n_columns_) {
            throw std::runtime_error("the insertion area exceeds the size of the buffer\n");
        }
        data_->get_data()[i * n_columns_ + j] = value;
    }

private:
    void swap(Matrix & rhs) noexcept {
        std::swap(n_rows_, rhs.n_rows_);
        std::swap(n_columns_, rhs.n_columns_);
        std::swap(data_, rhs.data_);
    }    

    std::size_t find_row_with_max_abs_value_in_column(std::size_t column) const {
        T* data = data_->get_data();

        std::size_t p = column;
        T best = std::abs(data[column * n_columns_ + column]);

        for (std::size_t i = column + 1; i < n_rows_; ++i) {
            T cur = std::abs(data[i * n_columns_ + column]);
            if (cur > best) {
                best = cur;
                p = i;
            }
        }
        return p;
    }

};

} // namespace matrix

#endif // INCLUDE_MATRIX_HPP
