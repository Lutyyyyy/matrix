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

template <class T> void destroy(T *p) { p->~T(); }

template <typename Iter> void destroy(Iter first, Iter last) {
    while (first != last)
        destroy(&*first++);
}


template <typename T> struct Buffer_t {

protected:    
    T* buffer_;
    size_t buffer_size_ = 0;
    size_t used_ = 0;

protected:    
    Buffer_t (size_t sz);
    
    Buffer_t (const Buffer_t &) = delete;
    Buffer_t &operator= (const Buffer_t &) = delete;
    
    Buffer_t (Buffer_t &&rhs) noexcept;
    Buffer_t &operator= (Buffer_t &&rhs) noexcept;
    
    ~Buffer_t();

}; //Buffer_t


template <typename T> struct Matrix_buffer_t final : private Buffer_t<T> {
private:
    using Buffer_t<T>::buffer_;
    using Buffer_t<T>::buffer_size_;
    using Buffer_t<T>::used_;
    
    size_t rows_ = 0;
    size_t columns_ = 0;
public:
    Matrix_buffer_t(size_t r, size_t c);
    Matrix_buffer_t(size_t r, size_t c, const std::vector<T> &input);
    
    Matrix_buffer_t(const Matrix_buffer_t<T> &rhs);
    Matrix_buffer_t &operator= (const Matrix_buffer_t &rhs);

    Matrix_buffer_t (Matrix_buffer_t &&rhs) = default;
    Matrix_buffer_t& operator= (Matrix_buffer_t &&rhs) = default;

    void push(const T &t);
    void push(T &&t);

    T& operator[] (size_t index) const; 

    size_t rows() const        { return rows_; }
    size_t columns() const     { return columns_; }
    size_t buffer_size() const { return buffer_size_; }
    size_t used() const        { return used_; }

    T* begin() const { return buffer_; }
    T* end() const { return buffer_ + buffer_size_; }

}; //Matrix_buffer_t


//============================================================================================================================

template <typename T> class Matrix_t final {

private:
    Matrix_buffer_t<T> buf_;
    Matrix_buffer_t<int> row_indices_;
public:
    
    Matrix_t (const size_t r = 0, const size_t c = 0);
    Matrix_t (const size_t r, const size_t c, const std::vector<T> &input);
    Matrix_t (const size_t r, const size_t c, const T &value);

    Matrix_t (const Matrix_t &rhs) = default;
    Matrix_t &operator= (const Matrix_t &rhs) = default;
    
    Matrix_t (Matrix_t &&rhs) = default;
    Matrix_t &operator= (Matrix_t &&rhs) = default;
    
    ~Matrix_t() = default;

    size_t rows() const { return buf_.rows(); }
    size_t columns() const { return buf_.columns(); }
    
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
    

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c) : buf_{r, c}, row_indices_{r, 1} {}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c, const std::vector<T> &input) : buf_{r, c, input}, row_indices_{r, 1} {
    for (size_t i = 0; i < r; ++i) {
        row_indices_.push(i);
    }

}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c, const T &value) : buf_{r, c}, row_indices_{r, 1} {
    while (buf_.used() < buf_.buffer_size()) {
        buf_.push(value);
    }
    
    for (size_t i = 0; i < r; ++i) {
        row_indices_.push(i);
    }
}

template<typename T>
T* Matrices::Matrix_t<T>::operator[] (const size_t index) const {  
    assert(index < rows());
    return buf_.begin() + row_indices_[index] * columns();
}


template<typename T>
bool Matrices::Matrix_t<T>::swap_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows() || index2 >= rows() || index1 == index2)
        return false;

    std::swap (row_indices_[index1], row_indices_[index2]);
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::add_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows() || index2 >= rows() || index1 == index2)
        return false;

    for (size_t i = 0; i != columns(); ++i) {
        (*this)[index1][i] += (*this)[index2][i];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::subtract_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows() || index2 >= rows() || index1 == index2)
        return false;

    for (size_t i = 0; i != columns(); ++i) {
        (*this)[index1][i] -= (*this)[index2][i];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::swap_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns() || index2 >= columns() || index1 == index2)
        return false;

    for (size_t i = 0; i != rows(); ++i) {
        std::swap ((*this)[i][index1], (*this)[i][index2]);
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::add_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns() || index2 >= columns() || index1 == index2)
        return false;
    
    for (size_t i = 0; i != rows(); ++i) {
        (*this)[i][index1] += (*this)[i][index2];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::subtract_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns() || index2 >= columns() || index1 == index2)
        return false;
    
    for (size_t i = 0; i != rows(); ++i) {
        (*this)[i][index1] -= (*this)[i][index2];
    }
    return true;
}

template<typename T>
T Matrices::Matrix_t<T>::determinant() const {
    if (rows() != columns()) {
        throw std::runtime_error("Cannot compute determinant of non square matrix\n");
    }
    
    Matrix_t<T> temp = *this;
    T det;
    int sign = 1;
    
    size_t sz = temp.rows();
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

    for (size_t i = current; i != rows(); ++i) {
        for (size_t j = current; j != columns(); ++j) {
            if ( std::fabs ((*this)[i][j]) > std::fabs ((*this)[max.first][max.second]) ) {
                max = std::make_pair (i, j);
            }
        }
    }

    return max;
}

template<typename T>
bool Matrices::Matrix_t<T>::eliminate_from_under_rows (const size_t index) {

    size_t sz = (*this).rows();
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
Matrices::Matrix_buffer_t<T>::Matrix_buffer_t(size_t r, size_t c) : Buffer_t<T>(r * c), rows_(r), columns_(c) {}

template<typename T>
Matrices::Matrix_buffer_t<T>::Matrix_buffer_t(size_t r, size_t c, const std::vector<T> &input) : Buffer_t<T>(r * c), rows_(r), columns_(c) {
    if (input.size() < buffer_size_) {
        throw std::runtime_error("Not enough data to construct matrix\n");
    }

    while(used_ < buffer_size_) {
        construct(buffer_ + used_, input[used_]);
        used_ += 1;
    }
}

template<typename T>
Matrices::Matrix_buffer_t<T>::Matrix_buffer_t(const Matrix_buffer_t<T> &rhs) : Buffer_t<T>(rhs.buffer_size_), rows_(rhs.rows_), columns_(rhs.columns_) {
    while (used_ < rhs.used_) {
        construct(buffer_ + used_, rhs.buffer_[used_]);
        used_ += 1;
    }
}

template<typename T>
Matrices::Matrix_buffer_t<T>& Matrices::Matrix_buffer_t<T>::operator= (const Matrix_buffer_t &rhs) {
    Matrix_buffer_t temp(rhs);
    std::swap(*this, temp);
    return *this;
}

template<typename T>
void Matrices::Matrix_buffer_t<T>::push(const T &t) {
    T t2(t);
    push(std::move(t2));
}

template<typename T>
void Matrices::Matrix_buffer_t<T>::push(T &&t) {
    assert(used_ < buffer_size_);
    static_assert(std::is_nothrow_move_constructible<T>::value);
    static_assert(std::is_nothrow_move_assignable<T>::value);

    construct(buffer_ + used_, std::move(t));
    used_ += 1;
}

template<typename T>
T& Matrices::Matrix_buffer_t<T>::operator[] (size_t index) const {
    return buffer_[index];
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
