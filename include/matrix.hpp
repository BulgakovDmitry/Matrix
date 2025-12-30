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

        std::size_t size = n_rows_;

        if (size == 0) return (T)1;
        if (size == 1) return *(data_->get_data());

        Matrix tmp(*this);
        T* a = tmp.data_->get_data();

        auto idx = [size](std::size_t i, std::size_t j) -> std::size_t {
            return i * size + j;
        };

        int sign = 1;

        for (std::size_t k = 0; k < size; ++k) {
            // ищем строку с максимальным |a[i][k]|
            std::size_t p = k;
            T best = std::abs(a[idx(k, k)]);
            for (std::size_t i = k + 1; i < size; ++i) {
                const T cur = std::abs(a[idx(i, k)]);
                if (cur > best) {
                    best = cur;
                    p = i;
                }
            }

            if (cmp::is_zero(best)) {
                return T(0);
            }

            // swap rows => меняется знак det
            if (p != k) {
                for (std::size_t j = 0; j < size; ++j) {
                    std::swap(a[idx(k, j)], a[idx(p, j)]);
                }
                sign = -sign;
            }

            const T pivot = a[idx(k, k)];
            if (cmp::is_zero(pivot)) {
                return T(0);
            }

            // зануляем элементы ниже диагонали
            for (std::size_t i = k + 1; i < size; ++i) {
                const T factor = a[idx(i, k)] / pivot;
                a[idx(i, k)] = T(0);
                for (std::size_t j = k + 1; j < size; ++j) {
                    a[idx(i, j)] -= factor * a[idx(k, j)];
                }
            }
        }

        T det = (sign > 0) ? T(1) : T(-1);
        for (std::size_t i = 0; i < size; ++i) {
            det *= a[idx(i, i)];
        }
        return det;
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
};

} // namespace matrix

#endif // INCLUDE_MATRIX_HPP
