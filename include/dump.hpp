#ifndef INCLUDE_DUMP_HPP
#define INCLUDE_DUMP_HPP

#include <ostream>
#include <matrix.hpp>

namespace matrix {

template<typename T>
void matrix_dump(const Matrix<T>& matrix, std::ostream &os) {
    os << "Matrix dump: {\n";

    std::size_t n_rows = matrix.get_n_rows();
    std::size_t n_columns = matrix.get_n_columns();
    for (std::size_t i = 0; i < n_rows; ++i) {
        for (std::size_t j = 0; j < n_columns) {
            os << '\t' << matrix.get_data()[i][j];
        }
        os << '\n';
    }
}

} // namespace matrix

#endif // INCLUDE_DUMP_HPP