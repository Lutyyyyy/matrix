#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>

namespace Matrices 
{

const double EPS = 1e-9;    
bool equal_eps (double a, double b) { return (std::fabs(a - b) < EPS); }


template <typename T> void construct (T *p, const T &rhs) { new (p) T(rhs); }
template <typename T> void construct (T *p, T &&rhs) { new (p) T(std::move(rhs)); }

template <class T> void destroy(T *p) { p->~T(); }

template <typename Iter> void destroy(Iter first, Iter last) {
    while (first != last)
        destroy(&*first++);
}

template <typename T> struct Buffer_t {

private:    
    T* buffer_;
    size_t buffer_size_ = 0;
    size_t used_ = 0;

public:    
    Buffer_t (size_t sz);
    
    Buffer_t (const Buffer_t &) = delete;
    Buffer_t &operator= (const Buffer_t &) = delete;
    
    Buffer_t (Buffer_t &&rhs) noexcept;
    Buffer_t &operator= (Buffer_t &&rhs) noexcept;
    
    ~Buffer_t();

    void push(const T &t);
    void push(T &&t);

    size_t size() const { return buffer_size_; }
    size_t used() const { return used_; }

    T* begin() const { return buffer_; }
    T* end() const { return buffer_ + buffer_size_; }
}; //Buffer_t


template <typename T> class Matrix_t final {

private:
    Buffer_t<T> buf_;
    
    size_t rows_ = 0;
    size_t columns_ = 0;
    std::vector<size_t> row_indices_;

public:

    Matrix_t (const size_t r, const size_t c, const std::vector<T> &input);
    Matrix_t (const size_t r, const size_t c, const T &value);

    Matrix_t (const Matrix_t &rhs);
    Matrix_t &operator= (const Matrix_t &rhs);
    
    Matrix_t (Matrix_t &&rhs) = default;
    Matrix_t &operator= (Matrix_t &&rhs) = default;
    
    ~Matrix_t() = default;

    size_t rows() const { return rows_; }
    size_t columns() const { return columns_; }
    size_t buf_size() const { return buf_.size(); }
    size_t row_index(size_t index) const;
    
    T* operator[] (const size_t index) const;


    T determinant() const;

    std::pair<size_t, size_t> max_element_of_submatrix (size_t current);
    bool eliminate_from_under_rows (const size_t index);

    bool swap_rows        (const size_t index1, const size_t index2);
    bool add_rows         (const size_t index1, const size_t index2);
    bool subtract_rows    (const size_t index1, const size_t index2);

    bool swap_columns     (const size_t index1, const size_t index2);
    bool add_columns      (const size_t index1, const size_t index2);
    bool subtract_columns (const size_t index1, const size_t index2);

}; //Matrix_t

} //Matrices
    
//============================================================================================================================

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c, const std::vector<T> &input) : buf_{r * c}, rows_{r}, columns_{c} {
    
    if (input.size() < buf_.size()) {
        throw std::runtime_error("Not enough data to construct matrix\n");
    }
    
    while(buf_.used() < buf_.size()) {
        buf_.push(input[buf_.used()]);
    }
    
    for (size_t i = 0; i < r; ++i) {
        row_indices_.push_back(i);
    }
}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c, const T &value) : buf_{r * c}, rows_{r}, columns_{c} {
    while (buf_.used() < buf_.size()) {
        buf_.push(value);
    }
    
    for (size_t i = 0; i < r; ++i) {
        row_indices_.push_back(i);
    }
}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t(const Matrix_t<T> &rhs) : buf_{rhs.buf_size()}, rows_{rhs.rows()}, columns_{rhs.columns()} {
    for (size_t i = 0; i < rhs.rows(); ++i) {
        for (size_t j = 0; j < rhs.columns(); ++j) {
            buf_.push(rhs[i][j]);
        }
        row_indices_.push_back(rhs.row_index(i));
    }
}

template<typename T>
Matrices::Matrix_t<T>& Matrices::Matrix_t<T>::operator= (const Matrix_t &rhs) {
    Matrix_t temp(rhs);
    std::swap(*this, temp);
    return *this;
}

template<typename T>
size_t Matrices::Matrix_t<T>::row_index(size_t index) const { 
    assert (index < rows_);
    return row_indices_[index]; 
}

template<typename T>
T* Matrices::Matrix_t<T>::operator[] (const size_t index) const {  
    assert(index < rows_);
    return buf_.begin() + row_indices_[index] * columns();
}

template<typename T>
bool Matrices::Matrix_t<T>::swap_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows_ || index2 >= rows_ || index1 == index2)
        return false;

    std::swap (row_indices_[index1], row_indices_[index2]);
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::add_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows_ || index2 >= rows_ || index1 == index2)
        return false;

    for (size_t i = 0; i < columns_; ++i) {
        (*this)[index1][i] += (*this)[index2][i];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::subtract_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows_ || index2 >= rows_ || index1 == index2)
        return false;

    for (size_t i = 0; i < columns_; ++i) {
        (*this)[index1][i] -= (*this)[index2][i];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::swap_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns_ || index2 >= columns_ || index1 == index2)
        return false;

    for (size_t i = 0; i < rows_; ++i) {
        std::swap ((*this)[i][index1], (*this)[i][index2]);
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::add_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns_ || index2 >= columns_ || index1 == index2)
        return false;
    
    for (size_t i = 0; i < rows_; ++i) {
        (*this)[i][index1] += (*this)[i][index2];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::subtract_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns_ || index2 >= columns_ || index1 == index2)
        return false;
    
    for (size_t i = 0; i < rows_; ++i) {
        (*this)[i][index1] -= (*this)[i][index2];
    }
    return true;    
}

template<typename T>
T Matrices::Matrix_t<T>::determinant() const {
    if (rows_ < columns_) {
        throw std::runtime_error("Cannot compute determinant of non square matrix\n");
    }
    
    Matrix_t<T> temp = *this;
    T det;
    int sign = 1;
    
    size_t sz = temp.rows_;

    if (sz == 1) return temp[0][0];
    for (size_t i = 0; i < sz - 1; i++) {

        std::pair<size_t, size_t> pivot = temp.max_element_of_submatrix(i);

        bool is_swap = temp.swap_rows (i, pivot.first);
        if (is_swap)
            sign = -sign;
        is_swap = temp.swap_columns (i, pivot.second);
        if (is_swap)
            sign = -sign;

        temp.eliminate_from_under_rows(i); 

        (i == 0) ? det = temp[i][i] : det *= temp[i][i];
    }

    det *= temp[sz - 1][sz - 1];

    if (equal_eps(det, 0))
        return 0;
    
    return sign * det;       
}

template<typename T>
std::pair<size_t, size_t> Matrices::Matrix_t<T>::max_element_of_submatrix (size_t current) {
    std::pair<size_t, size_t> max = std::make_pair(current, current);

    for (size_t i = current; i < rows_; ++i) {
        for (size_t j = current; j < columns_; ++j) {
            if ( std::fabs ((*this)[i][j]) > std::fabs ((*this)[max.first][max.second]) ) {
                max = std::make_pair (i, j);
            }
        }
    }

    return max;
}

template<typename T>
bool Matrices::Matrix_t<T>::eliminate_from_under_rows (const size_t index) {

    size_t sz = (*this).rows_;
    if (index >= sz) {
        return false;
    }

    for (size_t k = index + 1; k < sz; k++) {
        if ( !equal_eps ((*this)[k][index], 0.0) ) {
            T subtraction_coeff = (*this)[k][index] / (*this)[index][index];
            
            for (size_t j = index; j < sz; j++) {
                (*this)[k][j] -= ((*this)[index][j] * subtraction_coeff);
            }
        }
    }

    return true;
}

template<typename T>
Matrices::Buffer_t<T>::Buffer_t (Matrices::Buffer_t<T> &&rhs) noexcept : buffer_(rhs.buffer_), buffer_size_(rhs.buffer_size_), used_(rhs.used_) {
    rhs.buffer_ = nullptr;        
    rhs.buffer_size_ = 0;
    rhs.used_ = 0;
}

template<typename T>
Matrices::Buffer_t<T>& Matrices::Buffer_t<T>::operator= (Matrices::Buffer_t<T> &&rhs) noexcept {
    std::swap(buffer_, rhs.buffer_);
    std::swap(buffer_size_, rhs.buf_size_);
    std::swap(used_, rhs.used_);
    return *this;
}

template<typename T>
Matrices::Buffer_t<T>::Buffer_t (size_t sz) 
        : buffer_(sz == 0 ? nullptr : static_cast<T*>(::operator new(sizeof(T) * sz))) {
    buffer_size_ = sz;
}

template<typename T>
Matrices::Buffer_t<T>::~Buffer_t() {
    if (buffer_) {
        destroy (buffer_, buffer_ + used_);
        ::operator delete(buffer_);
    }
}

template<typename T>
void Matrices::Buffer_t<T>::push(const T &t) {
    T t2(t);
    push(std::move(t2));
}

template<typename T>
void Matrices::Buffer_t<T>::push(T &&t) {
    assert(used_ < buffer_size_);
    static_assert(std::is_nothrow_move_constructible<T>::value);
    static_assert(std::is_nothrow_move_assignable<T>::value);

    construct(buffer_ + used_, std::move(t));
    used_ += 1;
}


