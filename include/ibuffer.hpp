#ifndef INCLUDE_IBUFFER_HPP
#define INCLUDE_IBUFFER_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>

namespace matrix {

template<typename T>
struct IBuffer { 
    virtual ~IBuffer() = default;

    virtual const T*    get_data()     const noexcept = 0;
    virtual std::size_t get_size()     const noexcept = 0;
    virtual std::size_t get_capacity() const noexcept = 0;
};

} // namespace matrix

#endif // INCLUDE_IBUFFER_HPP