#ifndef INCLUDE_DUMP_HPP
#define INCLUDE_DUMP_HPP

#include <ostream>
#include "imatrix.hpp"

namespace matrix {

template<typename T>
void matrix_dump(const IMatrix<T>& matrix, std::ostream &os) {
    os << "Matrix dump: {\n";

    const std::size_t n_rows = matrix.get_n_rows();
    const std::size_t n_columns = matrix.get_n_columns();
    const T* data = matrix.get_data();

    for (std::size_t i = 0; i < n_rows; ++i) {
        for (std::size_t j = 0; j < n_columns; ++j) {
            os << '\t' << data[i * n_columns + j];
        }
        os << '\n';
    }
    os << "}\n";
}

} // namespace matrix

#endif // INCLUDE_DUMP_HPP