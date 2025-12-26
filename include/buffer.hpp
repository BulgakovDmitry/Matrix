#ifndef INCLUDE_BUFFER_HPP
#define INCLUDE_BUFFER_HPP

#include "ibuffer.hpp"

#include <cstddef>
#include <utility> 
#include <memory>
#include <new> 

namespace matrix {

template<typename T>
class Buffer final : public IBuffer<T> {    
private:
    T* data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};

public:
    Buffer() = default;

    explicit Buffer(std::size_t n_cells) 
        : data_{!n_cells ? nullptr : static_cast<T*>(::operator new(n_cells * sizeof(T)))}
        , size_{0}
        , capacity_{n_cells}
        {}
    
    Buffer(std::size_t n_rows, std::size_t n_columns)
        : data_{!(n_rows && n_columns) ? nullptr : static_cast<T*>(::operator new(n_columns * n_rows * sizeof(T)))}
        , size_{0}
        , capacity_{n_columns * n_rows}
        {}

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& rhs) noexcept { swap(rhs); }
    Buffer& operator=(Buffer&& rhs) noexcept { 
        swap(rhs);
        return *this;
    } 

    /*——————————————————————————————————————— IBuffer ———————————————————————————————————————————*/
    ~Buffer() override {
        std::destroy(data_, data_ + size_);
        ::operator delete(data_);
    }

          T*    get_data()           noexcept override { return data_;     }
    const T*    get_data()     const noexcept override { return data_;     }

    std::size_t get_size()     const noexcept override { return size_;     }
    std::size_t get_capacity() const noexcept override { return capacity_; }
    /*———————————————————————————————————————————————————————————————————————————————————————————*/

private:
    void swap(Buffer & rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
    }
};

} // namespace matrix

#endif // INCLUDE_BUFFER_HPP