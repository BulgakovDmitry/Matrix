#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include "buffer/ibuffer.hpp"
#include "buffer/buffer.hpp"
#include "matrix/imatrix.hpp"
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
        return *this;
    }

    /*——————————————————————————————————————— IMatrix ———————————————————————————————————————————*/
    ~Matrix() override = default;
    
    [[nodiscard]] std::size_t get_n_rows()    const noexcept { return n_rows_; }
    [[nodiscard]] std::size_t get_n_columns() const noexcept { return n_columns_; }
    [[nodiscard]] std::size_t get_size()      const noexcept { return n_rows_ * n_columns_; }

    [[nodiscard]] const T* get_data() const noexcept { return data_->get_data(); }
    [[nodiscard]]       T* get_data()       noexcept { return data_->get_data(); }
    /*———————————————————————————————————————————————————————————————————————————————————————————*/

    const T* operator[](std::size_t i) const noexcept {
        return data_->get_data() + i * n_columns_;
    }

    T* operator[](std::size_t i) noexcept {
        return data_->get_data() + i * n_columns_;
    }

    [[nodiscard]] bool is_square() const noexcept { return n_rows_ == n_columns_; }

    T calculate_determinant() const {
        if (!is_square()) throw std::runtime_error("determinant of non-square matrix");

        if (n_rows_ == 0) return T{1};
        if (n_rows_ == 1) return (*this)[0][0];

        Matrix tmp(*this);
        bool perms_even = true;
        tmp.convert_matrix_to_up_triang_form(std::addressof(perms_even));

        T det = T{1};
        for (std::size_t i = 0; i < n_rows_; ++i) det *= tmp[i][i];
        if (cmp::is_zero(det)) return T{0};
        if (!perms_even) det = -det;

        return det;
    }

    void insert(std::size_t i, std::size_t j, const T& value) {
        if (i >= n_rows_ || j >= n_columns_) {
            throw std::runtime_error("the insertion area exceeds the size of the buffer\n");
        }
        (*this)[i][j] = value;
    }
    
    Matrix transpose() const {
        Matrix tmp(n_columns_, n_rows_);

        for (std::size_t i = 0; i < n_rows_; ++i) {
            for (std::size_t j = 0; j < n_columns_; ++j) {
                tmp[j][i] = (*this)[i][j];
            }
        }
        return tmp;
    }

private:
    void swap(Matrix & rhs) noexcept {
        std::swap(n_rows_, rhs.n_rows_);
        std::swap(n_columns_, rhs.n_columns_);
        std::swap(data_, rhs.data_);
    }    

    void swap_rows(std::size_t row_1, std::size_t row_2) {
        if (row_1 == row_2) return;
        if (row_1 >= n_rows_ || row_2 >= n_rows_) {
            throw std::out_of_range("swap_rows: row index out of range");
        }

        for (std::size_t j = 0; j < n_columns_; ++j) {
            std::swap((*this)[row_1][j], 
                      (*this)[row_2][j]);
        }
    }

    void swap_columns(std::size_t col_1, std::size_t col_2) {
        if (col_1 == col_2) return;
        if (col_1 >= n_columns_ || col_2 >= n_columns_) {
            throw std::out_of_range("swap_columns: column index out of range");
        }

        for (std::size_t i = 0; i < n_rows_; ++i) {
            std::swap((*this)[i][col_1],
                      (*this)[i][col_2]);
        }
    }

    std::size_t find_elem_with_max_modulus_in_column(std::size_t column, 
                                                     std::size_t start_row = 0) const {
        if (column >= n_columns_) { throw std::runtime_error("out of range");          }
        if (n_rows_ == 0)         { throw std::runtime_error("empty matrix");          }
        if (start_row >= n_rows_) { throw std::out_of_range("start_row out of range"); }   

        std::size_t pivot_index = start_row;
        T pivot = std::abs((*this)[start_row][column]);
        for (std::size_t i = start_row + 1; i < n_rows_; ++i) {
            T candidate = std::abs((*this)[i][column]);
            if (cmp::greater<T>(candidate, pivot)) {
                pivot = candidate;
                pivot_index = i;
            }
        }
        return pivot_index;
    }

    Matrix<T>& convert_matrix_to_up_triang_form(bool* even_num_of_perms_flag = nullptr) {
        std::size_t number_of_permutations = 0;
        const std::size_t n = std::min(n_rows_, n_columns_);
        for (std::size_t i = 0; i < n; ++i) {
            /*———— выберу pivot такой, что в текущей колонке он по модулю максимальный ——————————*/
            std::size_t pivot_index = find_elem_with_max_modulus_in_column(i, i);
            /*———————————————————————————————————————————————————————————————————————————————————*/
            
            /*———— переносим pivot-строку на диагональ ——————————————————————————————————————————*/
            if (pivot_index != i) {
                swap_rows(pivot_index, i);
                ++number_of_permutations;
            }
            /*———————————————————————————————————————————————————————————————————————————————————*/

            /*———— проверка на нулевой pivot ————————————————————————————————————————————————————*/
            const T pivot = (*this)[i][i];
            if (cmp::is_zero(pivot)) { 
                continue;
            }
            /*———————————————————————————————————————————————————————————————————————————————————*/

            /*———— алгоритмом Гаусса приведу текущую колонку к верхнетреуг виду —————————————————*/
            for (std::size_t j = i + 1; j < n_rows_; ++j) {
                if (cmp::is_zero((*this)[j][i])) continue; 

                const T factor = (*this)[j][i] / pivot;   

                for (std::size_t k = i; k < n_columns_; ++k) {
                    (*this)[j][k] -= factor * (*this)[i][k];
                }

                (*this)[j][i] = T{0};
            }
            /*———————————————————————————————————————————————————————————————————————————————————*/
        }

        if (even_num_of_perms_flag) {
            *even_num_of_perms_flag = (number_of_permutations % 2 == 0);
        }
        return *this;
    }

};

} // namespace matrix

#endif // INCLUDE_MATRIX_HPP
