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
        , data_(rhs.data_ ? rhs.data->clone() : nullptr)
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

    template<typename T>
    T calculate_determinant() const {
        if (!is_square()) {
            throw std::runtime_error("it is impossible to calculate the determinant of a non-square matrix\n");
        }

        std::size_t size = n_rows_;

        if (size == 0) return static_cast<T>(1);
        if (size == 1) return *(data_->get_data());

        Matrix tmp(*this);
        T* a = tmp.data_->get_data();

        
        //TODO
        
        return 1;
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

    enum class Sign : bool {positive, negative};
    
};

} // namespace matrix

#endif // INCLUDE_MATRIX_HPP
