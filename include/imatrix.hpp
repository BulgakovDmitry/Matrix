#ifndef INCLUDE_IMATRIX_HPP
#define INCLUDE_IMATRIX_HPP

#include "ibuffer.hpp"
#include "buffer.hpp"

namespace matrix {

template<std::floating_point T>
struct IMatrix {
    virtual ~IMatrix() = default;

    /*——————————————————————————————————————— getters ———————————————————————————————————————————*/
    [[nodiscard]] virtual std::size_t get_n_rows()    const noexcept = 0;
    [[nodiscard]] virtual std::size_t get_n_columns() const noexcept = 0;
    [[nodiscard]] virtual std::size_t get_size()      const noexcept = 0;

    [[nodiscard]] virtual const T* get_data() const noexcept = 0;
    [[nodiscard]] virtual       T* get_data()       noexcept = 0;
    /*———————————————————————————————————————————————————————————————————————————————————————————*/
};

} // namespace matrix

#endif // INCLUDE_IMATRIX_HPP