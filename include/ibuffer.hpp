#ifndef INCLUDE_IBUFFER_HPP
#define INCLUDE_IBUFFER_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>

namespace matrix {

template<std::floating_point T>
struct IBuffer { 
    virtual ~IBuffer() = default;

    /*——————————————————————————————————————— getters ———————————————————————————————————————————*/
    virtual T*          get_data()           noexcept = 0;
    virtual const T*    get_data()     const noexcept = 0;
    virtual std::size_t get_size()     const noexcept = 0;
    virtual std::size_t get_capacity() const noexcept = 0;
    /*———————————————————————————————————————————————————————————————————————————————————————————*/

    virtual std::unique_ptr<IBuffer<T>> clone() const = 0;
};

} // namespace matrix

#endif // INCLUDE_IBUFFER_HPP