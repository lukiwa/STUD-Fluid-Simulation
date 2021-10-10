#pragma once

#include <algorithm>
#include <iostream>

template <typename T> class Array2D {
    int _size;
    T* array;

public:
    explicit Array2D(int size)
        : _size(size)
    {
        array = new T[size * size];
        std::fill(array, array + (size * size), 0);
    }

    ~Array2D()
    {
        delete[] array;
    }

    Array2D(const Array2D& other)
    {
        _size = other._size;
        std::copy(other.array, other.array + other._size, array);
    }

    Array2D& operator=(const Array2D& other)
    {
        if (&other != this) {
            _size = other._size;
            std::copy(other.array, other.array + other._size, array);
        }
        return *this;
    }

    int GetSize() const
    {
        return _size;
    }

    T& operator()(int x, int y) const
    {
#ifdef DEBUG
        return at(x, y);
#else
        return array[x + y * _size];
#endif
    }

    T& operator()(int x, int y)
    {
#ifdef DEBUG
        return at(x, y);
#else
        return array[x + y * _size];
#endif
    }

    T& at(int x, int y) const
    {
        if (x >= _size) {
            throw std::out_of_range("Array out of bound");
        }
        if (y >= _size) {
            throw std::out_of_range("Array out of bound");
        }
        return array[x + y * _size];
    }

    T& at(int x, int y)
    {
        if (x >= _size) {
            throw std::out_of_range("Array out of bound");
        }
        if (y >= _size) {
            throw std::out_of_range("Array out of bound");
        }
        return array[x + y * _size];
    }
};
