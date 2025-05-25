#pragma once
#include "common.h"

template<typename T, std::size_t Size>
class Array
{
private:
    T* ptr_{nullptr};
public:
    Array() : ptr_{new T[Size]} {}

    Array(const Array& other)
    {
        ptr_ = new T[Size];
        for (std::size_t i = 0; i < Size; i++)
            ptr_[i] = other.ptr_[i];
    }

    Array& operator=(const Array& other)
    {
        if (this == &other)
            return *this;

        T* newData = new T[Size];
        for (std::size_t i = 0; i < Size; i++)
            newData[i] = other.ptr_[i];

        delete[] ptr_;
        ptr_ = newData;
        return *this;
    }

    Array(Array&& other)
    {
        ptr_ = std::exchange(other.ptr_, nullptr);
    }

    Array& operator=(Array&& other)
    {
        if (this == &other)
            return *this;
        
        ptr_ = std::exchange(other.ptr_, nullptr);
        return *this;
    }

    ~Array()
    {
        delete[] ptr_;
    }

    T& operator[](std::size_t Idx)
    {
        return ptr_[Idx];
    }

    void fill(const T& val)
    {
        for (std::size_t i = 0; i < Size; i++)
            ptr_[i] = val;
    }

    T& front() const { return ptr_[0]; }
    T& back() const { return ptr_[Size - 1]; }

    T* data() const { return ptr_; }
    T* begin() const { return ptr_; }
    T* end() const { return ptr_ + Size; }

    constexpr std::size_t size() const { return Size; }
    constexpr bool empty() const { return Size == 0; }
};