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

//safe construct function
template <typename T> void construct (T *p, const T &rhs) { new (p) T(rhs); }


template <typename T> class Buffer_t final {

private:    
    size_t rows_ = 0;
    size_t columns_ = 0;
    T** buffer_;
    size_t buf_size_ = 0;

public:    
    Buffer_t (size_t r = 0, size_t c = 0);
    
    Buffer_t(const Buffer_t &) = delete;
    Buffer_t &operator=(const Buffer_t &) = delete;
    
    Buffer_t (Buffer_t &&rhs) noexcept;
    Buffer_t &operator= (Buffer_t &&rhs) noexcept;
    
    ~Buffer_t();

    size_t rows     () const { return rows_;    }
    size_t columns  () const { return columns_; }
    T**    buffer   () const { return buffer_; }
    size_t buf_size () const { return buf_size_; }
}; //Buffer_t


//============================================================================================================================

template <typename T> class Matrix_t final {

private:
    size_t rows_;
    size_t columns_;
    Buffer_t<T> buf_;
    size_t buf_size_;

public:
    
    Matrix_t (const size_t r = 0, const size_t c = 0);
    Matrix_t (const size_t r, const size_t c, const std::vector<T> &input);
    Matrix_t (const size_t r, const size_t c, const T &value);

    Matrix_t (const Matrix_t &rhs);
    Matrix_t &operator= (const Matrix_t &rhs);
    
    Matrix_t (Matrix_t &&rhs) = default;
    Matrix_t &operator= (Matrix_t &&rhs) = default;
    
    ~Matrix_t() = default;

    T* &operator[] (const size_t index) const;
    
    
    T determinant() const;


    std::pair<size_t, size_t> non_zero_element_of_submatrix (size_t current);
    std::pair<size_t, size_t> max_element_of_submatrix      (size_t current);


    bool swap_rows        (const size_t index1, const size_t index2);
    bool add_rows         (const size_t index1, const size_t index2);
    bool subtract_rows    (const size_t index1, const size_t index2);

    bool swap_columns     (const size_t index1, const size_t index2);
    bool add_columns      (const size_t index1, const size_t index2);
    bool subtract_columns (const size_t index1, const size_t index2);

}; //Matrix_t

} //Matrices
    

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c) 
        : buf_{r, c}, rows_(buf_.rows()), columns_(buf_.columns()), buf_size_(buf_.buf_size()) {}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c, const std::vector<T> &input) 
        : rows_(r), columns_(c), buf_{r, c}, buf_size_(r * c) {
    if (input.size() < buf_size_) {
        throw std::runtime_error("Not enough data to construct matrix\n");
    }
    
    for (size_t i = 0; i != r; ++i) {
        for (size_t j = 0; j != c; ++j) {
            construct((*this)[i] + j, input[i * c + j]);
        }
    }
}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const size_t r, const size_t c, const T &value) 
        : rows_(r), columns_(c), buf_{r, c}, buf_size_(r * c) {
    for (size_t i = 0; i != r; ++i) {
        for (size_t j = 0; j != c; ++j) {
            construct((*this)[i] + j, value);
        }
    }
}

template<typename T>
Matrices::Matrix_t<T>::Matrix_t (const Matrices::Matrix_t<T> &rhs) 
        : rows_(rhs.rows_), columns_(rhs.columns_), buf_{rhs.rows_, rhs.columns_}, buf_size_(rhs.buf_size_) {
    for (size_t i = 0; i != rhs.rows_; ++i) {
        for (size_t j = 0; j != rhs.columns_; ++j) {
            construct((*this)[i] + j, rhs[i][j]);
        }
    }
}

template<typename T>
Matrices::Matrix_t<T>& Matrices::Matrix_t<T>::operator= (const Matrices::Matrix_t<T> &rhs) {
    Matrix_t tmp{rhs};
    std::swap(*this, tmp);
    return *this;
}



template<typename T>
T*& Matrices::Matrix_t<T>::operator[] (const size_t index) const {  
    assert(index < rows_);
    return buf_.buffer()[index];
}


template<typename T>
bool Matrices::Matrix_t<T>::swap_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows_ || index2 >= rows_ || index1 == index2)
        return false;

    std::swap ((*this)[index1], (*this)[index2]);
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::add_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows_ || index2 >= rows_ || index1 == index2)
        return false;

    for (size_t i = 0; i != columns_; ++i) {
        (*this)[index1][i] += (*this)[index2][i];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::subtract_rows (const size_t index1, const size_t index2) {
    if (index1 >= rows_ || index2 >= rows_ || index1 == index2)
        return false;
    
    for (size_t i = 0; i != columns_; ++i) {
        (*this)[index1][i] -= (*this)[index2][i];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::swap_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns_ || index2 >= columns_ || index1 == index2)
        return false;

    for (size_t i = 0; i != rows_; ++i) {
        std::swap ((*this)[i][index1], (*this)[i][index2]);
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::add_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns_ || index2 >= columns_ || index1 == index2)
        return false;
    
    for (size_t i = 0; i != rows_; ++i) {
        (*this)[i][index1] += (*this)[i][index2];
    }
    return true;
}

template<typename T>
bool Matrices::Matrix_t<T>::subtract_columns (const size_t index1, const size_t index2) {
    if (index1 >= columns_ || index2 >= columns_ || index1 == index2)
        return false;
        
    for (size_t i = 0; i != rows_; ++i) {
        (*this)[i][index1] -= (*this)[i][index2];
    }
    return true;
}

template<typename T>
T Matrices::Matrix_t<T>::determinant() const {
    if (rows_ != columns_) {
        throw std::runtime_error("Cannot compute determinant of non square matrix\n");
    }

    Matrix_t<T> temp = *this;
    int sign = 1;

    size_t sz = rows_;
    for (size_t k = 0; k != sz - 1; ++k) {

        if (equal_eps(temp[k][k], 0.0) ) {
            std::pair<size_t, size_t> pivot = temp.max_element_of_submatrix(k);

            bool is_swap = temp.swap_rows (k, pivot.first);
            if (is_swap)
                sign = -sign;
            is_swap = temp.swap_columns (k, pivot.second);
            if (is_swap)
                sign = -sign;
        }

        if (equal_eps(temp[k][k], 0)) { return 0; }    

        for (size_t i = k + 1; i != sz; ++i) {
            for (size_t j = k + 1; j != sz; ++j) {       
                temp[i][j] = temp[k][k]*temp[i][j] - temp[i][k]*temp[k][j];
                if (k > 0) {
                    temp[i][j] /= temp[k-1][k-1];
                }
            }
        }
    }

    return sign * temp[sz-1][sz-1];
}

template<typename T>
std::pair<size_t, size_t> Matrices::Matrix_t<T>::max_element_of_submatrix (size_t current) {
    std::pair<size_t, size_t> max = std::make_pair(current, current);

    for (size_t i = current; i != rows_; ++i) {
        for (size_t j = current; j != columns_; ++j) {
            if ( std::fabs ((*this)[i][j]) > std::fabs ((*this)[max.first][max.second]) ) {
                max = std::make_pair (i, j);
            }
        }
    }

    return max;
}

template<typename T>
Matrices::Buffer_t<T>::Buffer_t (Matrices::Buffer_t<T> &&rhs) noexcept
        : rows_(rhs.rows_), columns_(rhs.columns_), buffer_(rhs.buffer_), buf_size_(rhs.buf_size_) {
    rhs.rows_ = 0;
    rhs.columns_ = 0;
    rhs.buffer_ = nullptr;        
    rhs.buf_size_ = 0;
}

template<typename T>
Matrices::Buffer_t<T>& Matrices::Buffer_t<T>::operator= (Matrices::Buffer_t<T> &&rhs) noexcept {
    std::swap(rows_ ,rhs.rows_);
    std::swap(columns_, rhs.columns_);
    std::swap(buffer_, rhs.buffer_);
    std::swap(buf_size_, rhs.buf_size_);

    return *this;
}

template<typename T>
Matrices::Buffer_t<T>::Buffer_t (size_t r, size_t c) 
        : buffer_((r == 0 || c == 0) ? nullptr : static_cast<T**>(::operator new(sizeof(T*) * r))) {
    while (rows_ < r) {
        rows_ += 1;
        buffer_[rows_-1] = new T[c];
    }
    columns_ = c;
    buf_size_ = rows_ * columns_;
}

template<typename T>
Matrices::Buffer_t<T>::~Buffer_t() {
    for (size_t i = 0; i != rows_; ++i) {
        delete[] buffer_[i];
    }
    ::operator delete(buffer_);
}
